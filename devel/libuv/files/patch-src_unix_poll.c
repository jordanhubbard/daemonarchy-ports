--- src/unix/poll.c.orig
+++ src/unix/poll.c
@@ -82,6 +82,22 @@
   if (err == UV_ENOTTY)
     err = uv__nonblock_fcntl(fd, 1);
 #endif
+#if defined(__FreeBSD__)
+  /* kqueue descriptors reject both ways of setting O_NONBLOCK.
+   * Confirm the descriptor with a zero-event, zero-timeout kevent call;
+   * this does not remove queued events or mask errors on other fds.
+   */
+  if (err == UV_ENOTTY) {
+    struct timespec timeout = {0, 0};
+    int rc;
+
+    do
+      rc = kevent(fd, NULL, 0, NULL, 0, &timeout);
+    while (rc == -1 && errno == EINTR);
+    if (rc == 0)
+      err = 0;
+  }
+#endif

   if (err)
     return err;
