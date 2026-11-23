--- lldb/packages/Python/lldbsuite/test/tools/lldb-dap/lldbdap_testcase.py.orig
+++ lldb/packages/Python/lldbsuite/test/tools/lldb-dap/lldbdap_testcase.py
@@ -495,7 +495,7 @@
     def getBuiltinDebugServerTool(self):
         # Tries to find simulation/lldb-server/gdbserver tool path.
         server_tool = None
-        if lldbplatformutil.getPlatform() == "linux":
+        if lldbplatformutil.getPlatform() in ("linux", "freebsd"):
             server_tool = lldbgdbserverutils.get_lldb_server_exe()
             if server_tool is None:
                 self.dap_server.request_disconnect(terminateDebuggee=True)
