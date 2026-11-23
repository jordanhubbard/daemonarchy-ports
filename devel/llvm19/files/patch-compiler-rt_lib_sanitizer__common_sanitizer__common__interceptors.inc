--- compiler-rt/lib/sanitizer_common/sanitizer_common_interceptors.inc.orig
+++ compiler-rt/lib/sanitizer_common/sanitizer_common_interceptors.inc
@@ -7356,8 +7356,14 @@
 #endif

 #if SANITIZER_INTERCEPT_READLINK
 INTERCEPTOR(SSIZE_T, readlink, const char *path, char *buf, SIZE_T bufsiz) {
+#if SANITIZER_FREEBSD
+  // libc can call readlink before the thread library is initialized.
+  // Avoid starting the sanitizer from that early startup call.
+  if (COMMON_INTERCEPTOR_NOTHING_IS_INITIALIZED)
+    return internal_readlink(path, buf, bufsiz);
+#endif
   void* ctx;
   COMMON_INTERCEPTOR_ENTER(ctx, readlink, path, buf, bufsiz);
   COMMON_INTERCEPTOR_READ_RANGE(ctx, path, internal_strlen(path) + 1);
   SSIZE_T res = REAL(readlink)(path, buf, bufsiz);
