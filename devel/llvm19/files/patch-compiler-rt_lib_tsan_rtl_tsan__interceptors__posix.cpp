--- compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp.orig
+++ compiler-rt/lib/tsan/rtl/tsan_interceptors_posix.cpp
@@ -1325,7 +1325,15 @@

 TSAN_INTERCEPTOR(int, pthread_mutex_init, void *m, void *a) {
   SCOPED_TSAN_INTERCEPTOR(pthread_mutex_init, m, a);
-  int res = REAL(pthread_mutex_init)(m, a);
+  int res;
+  {
+#if SANITIZER_FREEBSD
+    // libthr's private allocator reuses storage under uninstrumented locks.
+    // Its nested memset is not an access to the application's mutex handle.
+    ScopedIgnoreInterceptors ignore;
+#endif
+    res = REAL(pthread_mutex_init)(m, a);
+  }
   if (res == 0) {
     u32 flagz = 0;
     if (a) {
@@ -2809,16 +2817,33 @@
 #endif

 #if SANITIZER_FREEBSD
+TSAN_INTERCEPTOR(int, pthread_setspecific, unsigned key, const void *value) {
+  SCOPED_TSAN_INTERCEPTOR(pthread_setspecific, key, value);
+  // libthr may allocate its private key table using uninstrumented locks.
+  ScopedIgnoreInterceptors ignore;
+  return REAL(pthread_setspecific)(key, value);
+}
+#define TSAN_MAYBE_INTERCEPT_PTHREAD_SETSPECIFIC TSAN_INTERCEPT(pthread_setspecific)
+
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
 #define TSAN_MAYBE_INTERCEPT_THR_EXIT
-#endif
-
+#define TSAN_MAYBE_INTERCEPT_PTHREAD_SETSPECIFIC
+#endif
+
+TSAN_INTERCEPTOR_FREEBSD_ALIAS(int, setspecific, unsigned key, const void *value)
 TSAN_INTERCEPTOR_FREEBSD_ALIAS(int, cond_init, void *c, void *a)
 TSAN_INTERCEPTOR_FREEBSD_ALIAS(int, cond_destroy, void *c)
 TSAN_INTERCEPTOR_FREEBSD_ALIAS(int, cond_signal, void *c)
@@ -3051,6 +3076,7 @@

   TSAN_MAYBE_INTERCEPT__LWP_EXIT;
   TSAN_MAYBE_INTERCEPT_THR_EXIT;
+  TSAN_MAYBE_INTERCEPT_PTHREAD_SETSPECIFIC;

 #if !SANITIZER_APPLE && !SANITIZER_ANDROID
   // Need to setup it, because interceptors check that the function is resolved.
@@ -3074,6 +3100,7 @@
   }
 #endif

+  TSAN_MAYBE_INTERCEPT_FREEBSD_ALIAS(setspecific);
   TSAN_MAYBE_INTERCEPT_FREEBSD_ALIAS(cond_init);
   TSAN_MAYBE_INTERCEPT_FREEBSD_ALIAS(cond_destroy);
   TSAN_MAYBE_INTERCEPT_FREEBSD_ALIAS(cond_signal);
