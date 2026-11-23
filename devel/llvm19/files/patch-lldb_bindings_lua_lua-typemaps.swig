--- lldb/bindings/lua/lua-typemaps.swig.orig
+++ lldb/bindings/lua/lua-typemaps.swig
@@ -228,10 +228,22 @@
 
 //===----------------------------------------------------------------------===//
 
+// Prefer an exact native Lua file handle to the generic SBFile conversion.
+%typecheck(1) lldb::SBFile {
+  void *ptr;
+  if (lua_isuserdata(L, $input) == 0 ||
+      SWIG_ConvertPtr(L, $input, &ptr, $&descriptor, SWIG_POINTER_NO_NULL))
+    $1 = 0;
+  else
+    $1 = 1;
+}
+
 // Typemap for file handles (e.g. used in SBDebugger::SetOutputFile)
 
 %typemap(in) lldb::FileSP {
   luaL_Stream *p = (luaL_Stream *)luaL_checkudata(L, $input, LUA_FILEHANDLE);
+  if (!p->closef)
+    return luaL_error(L, "Invalid file");
   lldb::FileSP file_sp;
   file_sp = std::make_shared<lldb_private::NativeFile>(p->f, false);
   if (!file_sp->IsValid())
