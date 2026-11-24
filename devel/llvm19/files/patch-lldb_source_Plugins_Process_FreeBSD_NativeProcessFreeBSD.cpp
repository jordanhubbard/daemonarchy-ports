--- lldb/source/Plugins/Process/FreeBSD/NativeProcessFreeBSD.cpp.orig
+++ lldb/source/Plugins/Process/FreeBSD/NativeProcessFreeBSD.cpp
@@ -578,6 +578,12 @@
   }
 
   return PtraceWrapper(PT_KILL, m_pid);
+}
+
+void NativeProcessFreeBSD::DoStopIDBumped(uint32_t newBumpId) {
+  NativeProcessELF::DoStopIDBumped(newBumpId);
+  // Mappings and their permissions can change whenever the inferior runs.
+  m_mem_region_cache.clear();
 }
 
 Status NativeProcessFreeBSD::GetMemoryRegionInfo(lldb::addr_t load_addr,
