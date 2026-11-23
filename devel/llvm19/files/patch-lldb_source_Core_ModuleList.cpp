--- lldb/source/Core/ModuleList.cpp.orig
+++ lldb/source/Core/ModuleList.cpp
@@ -16,6 +16,7 @@
 #include "lldb/Interpreter/OptionValueProperties.h"
 #include "lldb/Interpreter/Property.h"
 #include "lldb/Symbol/ObjectFile.h"
+#include "lldb/Symbol/Symbol.h"
 #include "lldb/Symbol/SymbolContext.h"
 #include "lldb/Symbol/TypeList.h"
 #include "lldb/Symbol/VariableList.h"
@@ -32,6 +33,7 @@
 #endif
 
 #include "clang/Driver/Driver.h"
+#include "llvm/ADT/SmallPtrSet.h"
 #include "llvm/ADT/StringRef.h"
 #include "llvm/Support/FileSystem.h"
 #include "llvm/Support/Threading.h"
@@ -438,10 +440,66 @@
   return module_sp;
 }
 
+// An auxiliary provider only takes precedence for a symbol exported by its
+// filter. Moving whole modules would incorrectly promote unrelated symbols.
+static void AppendAuxiliarySymbol(const ModuleList &modules,
+                                  const SymbolContext &context,
+                                  SymbolContextList &results,
+                                  llvm::SmallPtrSetImpl<Module *> &visiting) {
+  Symbol *symbol = context.symbol;
+  ModuleSP module = context.module_sp;
+  if (!module || !symbol || symbol->GetType() == eSymbolTypeUndefined ||
+      (!symbol->IsExternal() && !symbol->IsWeak())) {
+    results.AppendIfUnique(context, false);
+    return;
+  }
+  if (!visiting.insert(module.get()).second)
+    return;
+
+  if (ObjectFile *object = module->GetObjectFile()) {
+    FileSpecList auxiliaries = object->GetAuxiliaryLibraries();
+    for (size_t i = 0; i < auxiliaries.GetSize(); ++i) {
+      ModuleSpec spec;
+      spec.GetFileSpec() = auxiliaries.GetFileSpecAtIndex(i);
+      ModuleSP provider = modules.FindFirstModule(spec);
+      if (!provider) {
+        spec.GetFileSpec().ClearDirectory();
+        provider = modules.FindFirstModule(spec);
+      }
+      if (!provider)
+        continue;
+
+      SymbolContextList matches;
+      provider->FindSymbolsWithNameAndType(symbol->GetMangled().GetName(),
+                                          eSymbolTypeAny, matches);
+      for (const SymbolContext &match : matches) {
+        if (!match.symbol ||
+            (!match.symbol->IsExternal() && !match.symbol->IsWeak()) ||
+            match.symbol->GetType() == eSymbolTypeUndefined)
+          continue;
+        AppendAuxiliarySymbol(modules, match, results, visiting);
+      }
+    }
+  }
+  visiting.erase(module.get());
+  // Retain the filter definition as the fallback when no provider defines it.
+  results.AppendIfUnique(context, false);
+}
+
+static void AppendAuxiliarySymbols(const ModuleList &modules,
+                                   const SymbolContextList &contexts,
+                                   SymbolContextList &results) {
+  for (const SymbolContext &context : contexts) {
+    llvm::SmallPtrSet<Module *, 4> visiting;
+    AppendAuxiliarySymbol(modules, context, results, visiting);
+  }
+}
+
 void ModuleList::FindFunctions(ConstString name,
                                FunctionNameType name_type_mask,
                                const ModuleFunctionSearchOptions &options,
-                               SymbolContextList &sc_list) const {
+                               SymbolContextList &results) const {
+  SymbolContextList sc_list;
   const size_t old_size = sc_list.GetSize();
 
   if (name_type_mask & eFunctionNameTypeAuto) {
@@ -464,11 +522,18 @@
                                options, sc_list);
     }
   }
+
+  if (options.include_symbols)
+    AppendAuxiliarySymbols(*this, sc_list, results);
+  else
+    for (const SymbolContext &context : sc_list)
+      results.AppendIfUnique(context, false);
 }
 
 void ModuleList::FindFunctionSymbols(ConstString name,
                                      lldb::FunctionNameType name_type_mask,
-                                     SymbolContextList &sc_list) {
+                                     SymbolContextList &results) {
+  SymbolContextList sc_list;
   const size_t old_size = sc_list.GetSize();
 
   if (name_type_mask & eFunctionNameTypeAuto) {
@@ -490,14 +555,23 @@
       module_sp->FindFunctionSymbols(name, name_type_mask, sc_list);
     }
   }
+
+  AppendAuxiliarySymbols(*this, sc_list, results);
 }
 
 void ModuleList::FindFunctions(const RegularExpression &name,
                                const ModuleFunctionSearchOptions &options,
-                               SymbolContextList &sc_list) {
+                               SymbolContextList &results) {
+  SymbolContextList sc_list;
   std::lock_guard<std::recursive_mutex> guard(m_modules_mutex);
   for (const ModuleSP &module_sp : m_modules)
     module_sp->FindFunctions(name, options, sc_list);
+
+  if (options.include_symbols)
+    AppendAuxiliarySymbols(*this, sc_list, results);
+  else
+    for (const SymbolContext &context : sc_list)
+      results.AppendIfUnique(context, false);
 }
 
 void ModuleList::FindCompileUnits(const FileSpec &path,
@@ -526,18 +600,24 @@
 
 void ModuleList::FindSymbolsWithNameAndType(ConstString name,
                                             SymbolType symbol_type,
-                                            SymbolContextList &sc_list) const {
+                                            SymbolContextList &results) const {
+  SymbolContextList sc_list;
   std::lock_guard<std::recursive_mutex> guard(m_modules_mutex);
   for (const ModuleSP &module_sp : m_modules)
     module_sp->FindSymbolsWithNameAndType(name, symbol_type, sc_list);
+
+  AppendAuxiliarySymbols(*this, sc_list, results);
 }
 
 void ModuleList::FindSymbolsMatchingRegExAndType(
     const RegularExpression &regex, lldb::SymbolType symbol_type,
-    SymbolContextList &sc_list) const {
+    SymbolContextList &results) const {
+  SymbolContextList sc_list;
   std::lock_guard<std::recursive_mutex> guard(m_modules_mutex);
   for (const ModuleSP &module_sp : m_modules)
     module_sp->FindSymbolsMatchingRegExAndType(regex, symbol_type, sc_list);
+
+  AppendAuxiliarySymbols(*this, sc_list, results);
 }
 
 void ModuleList::FindModules(const ModuleSpec &module_spec,
