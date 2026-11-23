--- lldb/source/Plugins/ObjectFile/ELF/ObjectFileELF.cpp.orig
+++ lldb/source/Plugins/ObjectFile/ELF/ObjectFileELF.cpp
@@ -865,6 +865,11 @@
   return num_specs;
 }
 
+FileSpecList ObjectFileELF::GetAuxiliaryLibraries() {
+  ParseDependentModules();
+  return m_auxiliary_libraries;
+}
+
 Address ObjectFileELF::GetImageInfoAddress(Target *target) {
   if (!ParseDynamicSymbols())
     return Address();
@@ -972,6 +977,10 @@
 
 // ParseDependentModules
 size_t ObjectFileELF::ParseDependentModules() {
+  ModuleSP module_sp(GetModule());
+  if (!module_sp)
+    return 0;
+  std::lock_guard<std::recursive_mutex> guard(module_sp->GetMutex());
   if (m_filespec_up)
     return m_filespec_up->GetSize();
 
@@ -1009,20 +1018,25 @@
     const lldb::offset_t section_size = dynsym_data.GetByteSize();
     lldb::offset_t offset = 0;
 
-    // The only type of entries we are concerned with are tagged DT_NEEDED,
-    // yielding the name of a required library.
+    // Keep required dependencies separate from optional auxiliary providers.
+    // The order of DT_AUXILIARY entries determines symbol lookup precedence.
     while (offset < section_size) {
       if (!symbol.Parse(dynsym_data, &offset))
         break;
 
-      if (symbol.d_tag != DT_NEEDED)
+      if (symbol.d_tag != DT_NEEDED && symbol.d_tag != DT_AUXILIARY)
         continue;
 
       uint32_t str_index = static_cast<uint32_t>(symbol.d_val);
       const char *lib_name = dynstr_data.PeekCStr(str_index);
+      if (!lib_name || !*lib_name)
+        continue;
       FileSpec file_spec(lib_name);
       FileSystem::Instance().Resolve(file_spec);
-      m_filespec_up->Append(file_spec);
+      if (symbol.d_tag == DT_AUXILIARY)
+        m_auxiliary_libraries.Append(file_spec);
+      else
+        m_filespec_up->Append(file_spec);
     }
   }
 
