--- lldb/source/Plugins/Process/gdb-remote/GDBRemoteCommunicationServerLLGS.cpp.orig
+++ lldb/source/Plugins/Process/gdb-remote/GDBRemoteCommunicationServerLLGS.cpp
@@ -2784,8 +2784,9 @@
                     region_info.GetRange().GetRangeBase(),
                     region_info.GetRange().GetByteSize());
 
-    // Permissions.
-    if (region_info.GetReadable() || region_info.GetWritable() ||
+    // Keep mapped regions distinct from holes even with no access permissions.
+    if (region_info.GetMapped() == MemoryRegionInfo::eYes ||
+        region_info.GetReadable() || region_info.GetWritable() ||
         region_info.GetExecutable()) {
       // Write permissions info.
       response.PutCString("permissions:");
