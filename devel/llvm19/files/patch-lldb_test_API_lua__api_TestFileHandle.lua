--- lldb/test/API/lua_api/TestFileHandle.lua.orig
+++ lldb/test/API/lua_api/TestFileHandle.lua
@@ -34,4 +34,32 @@
     f:close()
 end
 
+function _T:TestFileOverloadContracts()
+    for _, name in ipairs({'Input', 'Output', 'Error'}) do
+        local setter = self.debugger['Set' .. name .. 'File']
+        local getter = self.debugger['Get' .. name .. 'File']
+        local original = getter(self.debugger)
+        local f = assert(io.tmpfile())
+        assertTrue(pcall(setter, self.debugger, f))
+        local wrapped = getter(self.debugger)
+        assertTrue(wrapped:IsValid())
+        assertTrue(pcall(setter, self.debugger, wrapped))
+        assertTrue(getter(self.debugger):IsValid())
+
+        for _, invalid in ipairs({'not a file', 42, false, {}, lldb.SBError()}) do
+            assertFalse(pcall(setter, self.debugger, invalid))
+        end
+        assertFalse(pcall(setter, self.debugger, nil))
+
+        local closed = assert(io.tmpfile())
+        closed:close()
+        local success, message = pcall(setter, self.debugger, closed)
+        assertFalse(success)
+        assertStrContains(message, 'Invalid file')
+
+        setter(self.debugger, original)
+        f:close()
+    end
+end
+
 os.exit(_T:run())
