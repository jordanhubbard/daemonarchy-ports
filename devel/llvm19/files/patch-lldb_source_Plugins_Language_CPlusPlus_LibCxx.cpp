--- lldb/source/Plugins/Language/CPlusPlus/LibCxx.cpp.orig
+++ lldb/source/Plugins/Language/CPlusPlus/LibCxx.cpp
@@ -27,6 +27,7 @@
 #include "Plugins/LanguageRuntime/CPlusPlus/CPPLanguageRuntime.h"
 #include "Plugins/TypeSystem/Clang/TypeSystemClang.h"
 #include "lldb/lldb-enumerations.h"
+#include <cctype>
 #include <optional>
 #include <tuple>
 
@@ -74,6 +75,33 @@
     value = pair.GetChildMemberWithName("__second_");
   }
   return value;
+}
+
+std::pair<lldb::ValueObjectSP, bool>
+lldb_private::formatters::GetValueOrOldCompressedPair(
+    ValueObject &obj, llvm::StringRef child_name,
+    llvm::StringRef compressed_pair_name) {
+  auto is_old_compressed_pair = [](ValueObject &pair_obj) {
+    llvm::StringRef name = pair_obj.GetTypeName().GetStringRef();
+    if (name.consume_front("std::")) {
+      // Account for libc++ inline ABI namespaces such as __1 and __lldb.
+      auto inline_name = name;
+      if (inline_name.consume_front("__") && !inline_name.empty()) {
+        inline_name = inline_name.drop_while([](char c) {
+          return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
+        });
+        if (inline_name.consume_front("::"))
+          name = inline_name;
+      }
+    }
+    return name.consume_front("__compressed_pair") && name.starts_with("<");
+  };
+  if (ValueObjectSP child_sp = obj.GetChildMemberWithName(child_name))
+    return {child_sp, is_old_compressed_pair(*child_sp)};
+  ValueObjectSP pair_sp = obj.GetChildMemberWithName(compressed_pair_name);
+  if (pair_sp && is_old_compressed_pair(*pair_sp))
+    return {pair_sp, true};
+  return {nullptr, false};
 }
 
 bool lldb_private::formatters::LibcxxFunctionSummaryProvider(
@@ -172,11 +200,13 @@
   if (!valobj_sp)
     return false;
 
-  ValueObjectSP ptr_sp(valobj_sp->GetChildMemberWithName("__ptr_"));
-  if (!ptr_sp)
-    return false;
-
-  ptr_sp = GetFirstValueOfLibCXXCompressedPair(*ptr_sp);
+  auto [ptr_sp, is_compressed_pair] =
+      GetValueOrOldCompressedPair(*valobj_sp, "__ptr_", "__ptr_");
+  if (!ptr_sp)
+    return false;
+
+  if (is_compressed_pair)
+    ptr_sp = GetFirstValueOfLibCXXCompressedPair(*ptr_sp);
   if (!ptr_sp)
     return false;
 
@@ -357,19 +387,29 @@
   if (!valobj_sp)
     return lldb::ChildCacheState::eRefetch;
 
-  ValueObjectSP ptr_sp(valobj_sp->GetChildMemberWithName("__ptr_"));
+  auto [ptr_sp, is_compressed_pair] =
+      GetValueOrOldCompressedPair(*valobj_sp, "__ptr_", "__ptr_");
   if (!ptr_sp)
     return lldb::ChildCacheState::eRefetch;
 
   // Retrieve the actual pointer and the deleter, and clone them to give them
   // user-friendly names.
-  ValueObjectSP value_pointer_sp = GetFirstValueOfLibCXXCompressedPair(*ptr_sp);
-  if (value_pointer_sp)
-    m_value_ptr_sp = value_pointer_sp->Clone(ConstString("pointer"));
-
-  ValueObjectSP deleter_sp = GetSecondValueOfLibCXXCompressedPair(*ptr_sp);
-  if (deleter_sp)
-    m_deleter_sp = deleter_sp->Clone(ConstString("deleter"));
+  if (is_compressed_pair) {
+    if (ValueObjectSP value_pointer_sp =
+            GetFirstValueOfLibCXXCompressedPair(*ptr_sp))
+      m_value_ptr_sp = value_pointer_sp->Clone(ConstString("pointer"));
+
+    if (ValueObjectSP deleter_sp =
+            GetSecondValueOfLibCXXCompressedPair(*ptr_sp))
+      m_deleter_sp = deleter_sp->Clone(ConstString("deleter"));
+  } else {
+    m_value_ptr_sp = ptr_sp->Clone(ConstString("pointer"));
+
+    if (ValueObjectSP deleter_sp =
+            valobj_sp->GetChildMemberWithName("__deleter_"))
+      if (deleter_sp->GetNumChildrenIgnoringErrors() > 0)
+        m_deleter_sp = deleter_sp->Clone(ConstString("deleter"));
+  }
 
   return lldb::ChildCacheState::eRefetch;
 }
@@ -412,19 +452,11 @@
 // TODO: Support big-endian architectures.
 static std::optional<std::pair<uint64_t, ValueObjectSP>>
 ExtractLibcxxStringInfo(ValueObject &valobj) {
-  ValueObjectSP valobj_r_sp = valobj.GetChildMemberWithName("__r_");
-  if (!valobj_r_sp || !valobj_r_sp->GetError().Success())
-    return {};
-
-  // __r_ is a compressed_pair of the actual data and the allocator. The data we
-  // want is in the first base class.
-  ValueObjectSP valobj_r_base_sp = valobj_r_sp->GetChildAtIndex(0);
-  if (!valobj_r_base_sp)
-    return {};
-
-  ValueObjectSP valobj_rep_sp =
-      valobj_r_base_sp->GetChildMemberWithName("__value_");
-  if (!valobj_rep_sp)
+  auto [valobj_rep_sp, is_compressed_pair] =
+      GetValueOrOldCompressedPair(valobj, "__rep_", "__r_");
+  if (is_compressed_pair)
+    valobj_rep_sp = GetFirstValueOfLibCXXCompressedPair(*valobj_rep_sp);
+  if (!valobj_rep_sp || !valobj_rep_sp->GetError().Success())
     return {};
 
   ValueObjectSP l = valobj_rep_sp->GetChildMemberWithName("__l");
