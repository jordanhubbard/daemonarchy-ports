--- lldb/source/Plugins/ObjectFile/ELF/ObjectFileELF.h.orig
+++ lldb/source/Plugins/ObjectFile/ELF/ObjectFileELF.h
@@ -129,6 +129,8 @@
 
   uint32_t GetDependentModules(lldb_private::FileSpecList &files) override;
 
+  lldb_private::FileSpecList GetAuxiliaryLibraries() override;
+
   lldb_private::Address
   GetImageInfoAddress(lldb_private::Target *target) override;
 
@@ -223,6 +225,9 @@
   /// List of file specifications corresponding to the modules (shared
   /// libraries) on which this object file depends.
   mutable std::unique_ptr<lldb_private::FileSpecList> m_filespec_up;
+
+  /// Optional ELF auxiliary providers, retained in dynamic-tag order.
+  lldb_private::FileSpecList m_auxiliary_libraries;
 
   /// Cached value of the entry point for this module.
   lldb_private::Address m_entry_point_address;
