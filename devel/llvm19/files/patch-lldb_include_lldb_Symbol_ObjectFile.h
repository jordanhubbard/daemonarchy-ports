--- lldb/include/lldb/Symbol/ObjectFile.h.orig
+++ lldb/include/lldb/Symbol/ObjectFile.h
@@ -390,6 +390,11 @@
   virtual lldb_private::FileSpecList GetReExportedLibraries() {
     return FileSpecList();
   }
+
+  /// Libraries whose definitions take precedence over matching exported
+  /// symbols in this object, in loader lookup order (ELF DT_AUXILIARY).
+  /// Unlike required dependencies, an absent auxiliary library is optional.
+  virtual FileSpecList GetAuxiliaryLibraries() { return FileSpecList(); }
 
   /// Sets the load address for an entire module, assuming a rigid slide of
   /// sections, if possible in the implementation.
