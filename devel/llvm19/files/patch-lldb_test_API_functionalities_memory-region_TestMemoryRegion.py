--- lldb/test/API/functionalities/memory-region/TestMemoryRegion.py.orig
+++ lldb/test/API/functionalities/memory-region/TestMemoryRegion.py
@@ -137,6 +137,70 @@
 
                 previous_base = region_base
                 previous_end = region_end
+
+
+    @skipIfWindows
+    def test_memory_region_refresh_after_resume(self):
+        self.build(dictionary=dict(CXXFLAGS_EXTRAS="-DTEST_MEMORY_REGION_REFRESH"))
+        self.dbg.SetAsync(False)
+        target, process, thread, first_bp = lldbutil.run_to_name_breakpoint(
+            self, "stop_none"
+        )
+        stages = [
+            ("stop_none", True, False, False),
+            ("stop_rw", True, True, True),
+            ("stop_ro", True, True, False),
+            ("stop_unmapped", False, False, False),
+        ]
+        breakpoints = {"stop_none": first_bp}
+        for name, _, _, _ in stages[1:]:
+            bp = target.BreakpointCreateByName(name)
+            self.assertEqual(bp.GetNumLocations(), 1)
+            breakpoints[name] = bp
+
+        previous_stop = 0
+        for name, mapped, readable, writable in stages:
+            self.assertState(process.GetState(), lldb.eStateStopped)
+            threads = [
+                t for t in process
+                if t.GetStopReason() == lldb.eStopReasonBreakpoint
+                and t.GetStopReasonDataCount() >= 2
+                and t.GetStopReasonDataAtIndex(0) == breakpoints[name].GetID()
+            ]
+            self.assertEqual(len(threads), 1)
+            self.assertGreater(process.GetStopID(), previous_stop)
+            previous_stop = process.GetStopID()
+            pointer = target.FindFirstGlobalVariable("test_region")
+            length = target.FindFirstGlobalVariable("test_region_size")
+            self.assertTrue(pointer.IsValid())
+            self.assertTrue(length.IsValid())
+            self.assertSuccess(pointer.GetError())
+            self.assertSuccess(length.GetError())
+            address = pointer.GetValueAsUnsigned()
+            size = length.GetValueAsUnsigned()
+            self.assertNotEqual(address, 0)
+            self.assertNotEqual(address, lldb.LLDB_INVALID_ADDRESS)
+            self.assertGreater(size, 0)
+            info = lldb.SBMemoryRegionInfo()
+            self.assertSuccess(process.GetMemoryRegionInfo(address, info))
+            self.assertEqual(info.IsMapped(), mapped)
+            self.assertEqual(info.IsReadable(), readable)
+            self.assertEqual(info.IsWritable(), writable)
+            self.assertFalse(info.IsExecutable())
+            if mapped:
+                self.assertLessEqual(info.GetRegionBase(), address)
+                self.assertGreaterEqual(info.GetRegionEnd(), address + size)
+            if name in ["stop_rw", "stop_ro"]:
+                error = lldb.SBError()
+                self.assertEqual(process.ReadMemory(address, 1, error), b"\x2a")
+                self.assertSuccess(error)
+            if name == "stop_unmapped":
+                error = lldb.SBError()
+                process.ReadMemory(address, 1, error)
+                self.assertTrue(error.Fail())
+            process.Continue()
+        self.assertState(process.GetState(), lldb.eStateExited)
+        self.assertEqual(process.GetExitStatus(), 0)
 
 
 class MemoryCommandRegionAll(GDBRemoteTestBase):
