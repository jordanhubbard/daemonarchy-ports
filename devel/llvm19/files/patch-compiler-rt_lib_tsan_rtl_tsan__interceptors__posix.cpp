--- compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp.orig
+++ compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp
@@ -2809,10 +2809,17 @@
 #endif

 #if SANITIZER_FREEBSD
 TSAN_INTERCEPTOR(void, thr_exit, tid_t *state) {
-  SCOPED_TSAN_INTERCEPTOR(thr_exit, state);
-  DestroyThreadState();
+  {
+    SCOPED_INTERCEPTOR_RAW(thr_exit, state);
+    CHECK_REAL_FUNC(thr_exit);
+  }
+  // Balance the interceptor's own ignores before checking thread teardown.
+  // Forked children still need their allocated thread state released.
+  ThreadState *thr = cur_thread();
+  if (thr->is_inited && !thr->is_dead)
+    DestroyThreadState();
   REAL(thr_exit(state));
 }
 #define TSAN_MAYBE_INTERCEPT_THR_EXIT TSAN_INTERCEPT(thr_exit)
 #else
