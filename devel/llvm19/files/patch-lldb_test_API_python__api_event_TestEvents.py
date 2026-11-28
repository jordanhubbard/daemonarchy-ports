--- lldb/test/API/python_api/event/TestEvents.py.orig
+++ lldb/test/API/python_api/event/TestEvents.py
@@ -319,57 +319,63 @@
         """Wait for an event from self.primary & self.shadow listener.
         If test_shadow is true, we also check that the shadow listener only
         receives events AFTER the primary listener does."""
-        import stop_hook
-        # Waiting on the shadow listener shouldn't have events yet because
-        # we haven't fetched them for the primary listener yet:
-        event = lldb.SBEvent()
-
-        if test_shadow:
-            success = self.shadow_listener.WaitForEvent(1, event)
-            self.assertFalse(success, "Shadow listener doesn't pull events")
-
-        # But there should be an event for the primary listener:
-        success = self.primary_listener.WaitForEvent(5, event)
-
-        self.assertTrue(success, "Primary listener got the event")
-
-        state = lldb.SBProcess.GetStateFromEvent(event)
-        primary_event_type = event.GetType()
-        restart = False
-        if state == lldb.eStateStopped:
-            restart = lldb.SBProcess.GetRestartedFromEvent(event)
-            # This counter is matching the stop hooks, which don't get run
-            # for auto-restarting stops.
-            if not restart:
-                self.stop_counter += 1
+        # Process I/O notifications can carry the current stopped state.
+        # Match every primary/shadow event, but count hooks and return only
+        # for actual process state-change events.
+        while True:
+            import stop_hook
+            # Waiting on the shadow listener shouldn't have events yet because
+            # we haven't fetched them for the primary listener yet:
+            event = lldb.SBEvent()
+
+            if test_shadow:
+                success = self.shadow_listener.WaitForEvent(1, event)
+                self.assertFalse(success, "Shadow listener doesn't pull events")
+
+            # But there should be an event for the primary listener:
+            success = self.primary_listener.WaitForEvent(5, event)
+
+            self.assertTrue(success, "Primary listener got the event")
+
+            state = lldb.SBProcess.GetStateFromEvent(event)
+            primary_event_type = event.GetType()
+            state_changed = bool(primary_event_type & lldb.SBProcess.eBroadcastBitStateChanged)
+            restart = False
+            if state_changed and state == lldb.eStateStopped:
+                restart = lldb.SBProcess.GetRestartedFromEvent(event)
+                # This counter is matching the stop hooks, which don't get run
+                # for auto-restarting stops.
+                if not restart:
+                    self.stop_counter += 1
+                    self.assertEqual(
+                        stop_hook.StopHook.counter[self.instance],
+                        self.stop_counter,
+                        "matching stop hook",
+                    )
+
+            if state_changed and expected_state is not None:
                 self.assertEqual(
-                    stop_hook.StopHook.counter[self.instance],
-                    self.stop_counter,
-                    "matching stop hook",
+                    state, expected_state, "Primary thread got the correct event"
                 )
 
-        if expected_state is not None:
+            # And after pulling that one there should be an equivalent event for the shadow
+            # listener:
+            success = self.shadow_listener.WaitForEvent(5, event)
+            self.assertTrue(success, "Shadow listener got event too")
+            shadow_event_type = event.GetType()
             self.assertEqual(
-                state, expected_state, "Primary thread got the correct event"
+                primary_event_type, shadow_event_type, "It was the same event type"
             )
-
-        # And after pulling that one there should be an equivalent event for the shadow
-        # listener:
-        success = self.shadow_listener.WaitForEvent(5, event)
-        self.assertTrue(success, "Shadow listener got event too")
-        shadow_event_type = event.GetType()
-        self.assertEqual(
-            primary_event_type, shadow_event_type, "It was the same event type"
-        )
-        self.assertEqual(
-            state, lldb.SBProcess.GetStateFromEvent(event), "It was the same state"
-        )
-        self.assertEqual(
-            restart,
-            lldb.SBProcess.GetRestartedFromEvent(event),
-            "It was the same restarted",
-        )
-        return state, restart
+            self.assertEqual(
+                state, lldb.SBProcess.GetStateFromEvent(event), "It was the same state"
+            )
+            self.assertEqual(
+                restart,
+                lldb.SBProcess.GetRestartedFromEvent(event),
+                "It was the same restarted",
+            )
+            if state_changed:
+                return state, restart
 
     @expectedFlakeyLinux("llvm.org/pr23730")  # Flaky, fails ~1/100 cases
     @skipIfWindows  # This is flakey on Windows AND when it fails, it hangs: llvm.org/pr38373
@@ -485,3 +491,54 @@
         # Now make sure we agree with the stop hook counter:
         self.assertEqual(self.stop_counter, stop_hook.StopHook.counter[self.instance])
         self.assertEqual(stop_hook.StopHook.non_stops[self.instance], 0, "No non stops")
+
+    @skipIfWindows
+    @skipIfNetBSD
+    def test_shadow_listener_ignores_stdout(self):
+        """A pending stdout event is mirrored, but is not a process state change."""
+        self.build()
+        target = self.dbg.CreateTarget(self.getBuildArtifact("a.out"))
+        self.assertTrue(target, VALID_TARGET)
+        breakpoint = target.BreakpointCreateByName("main")
+        self.assertEqual(breakpoint.GetNumLocations(), 1, VALID_BREAKPOINT)
+        self.primary_listener = lldb.SBListener("stdout primary listener")
+        self.shadow_listener = lldb.SBListener("stdout shadow listener")
+        launch_info = target.GetLaunchInfo()
+        launch_info.SetListener(self.primary_listener)
+        launch_info.SetShadowListener(self.shadow_listener)
+        self.dbg.SetAsync(True)
+        self.instance = f"StdoutKey{random.randint(0,10000)}"
+        stop_hook_path = os.path.join(self.getSourceDir(), "stop_hook.py")
+        self.runCmd(f"command script import {stop_hook_path}")
+        import stop_hook
+        self.runCmd(
+            f"target stop-hook add -P stop_hook.StopHook -k instance -v {self.instance}"
+        )
+        self.stop_counter = 0
+        error = lldb.SBError()
+        self.process = target.Launch(launch_info, error)
+        self.assertSuccess(error, "Process launched successfully")
+        state = lldb.eStateInvalid
+        while state != lldb.eStateStopped:
+            state, _ = self.wait_for_next_event(None)
+        self.assertEqual(self.stop_counter, 1)
+
+        # Queue a real broadcaster notification before continuing. It must be
+        # mirrored to the shadow listener before the running state event, and
+        # must not satisfy a request to wait for that state event.
+        payload = "pending stdout event"
+        notification = lldb.SBEvent(
+            lldb.SBProcess.eBroadcastBitSTDOUT, payload
+        )
+        self.process.GetBroadcaster().BroadcastEvent(notification)
+        self.assertSuccess(self.process.Continue())
+        self.wait_for_next_event(lldb.eStateRunning)
+        state = lldb.eStateRunning
+        counter = 0
+        while state != lldb.eStateExited:
+            counter += 1
+            self.assertLess(counter, 50, "Took more than 50 state events to exit.")
+            state, _ = self.wait_for_next_event(None)
+        self.assertEqual(self.stop_counter, 1)
+        self.assertEqual(stop_hook.StopHook.counter[self.instance], 1)
+        self.assertEqual(stop_hook.StopHook.non_stops[self.instance], 0)
