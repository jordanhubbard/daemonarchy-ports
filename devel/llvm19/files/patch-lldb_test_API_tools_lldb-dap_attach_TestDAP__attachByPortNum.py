--- lldb/test/API/tools/lldb-dap/attach/TestDAP_attachByPortNum.py.orig
+++ lldb/test/API/tools/lldb-dap/attach/TestDAP_attachByPortNum.py
@@ -38,7 +38,7 @@
 
     def get_debug_server_command_line_args(self):
         args = []
-        if lldbplatformutil.getPlatform() == "linux":
+        if lldbplatformutil.getPlatform() in ("linux", "freebsd"):
             args = ["gdbserver"]
         elif lldbplatformutil.getPlatform() == "macosx":
             args = ["--listen"]
