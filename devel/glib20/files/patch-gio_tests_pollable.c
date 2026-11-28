Backport upstream GLib FreeBSD test portability fixes.

Obtained from: https://github.com/GNOME/glib/commit/c18fa6e4b0d309366d07a8932686b674448d6fd9
Obtained from: https://github.com/GNOME/glib/commit/3983b3e22aa00344bf1862c0360d870e4a1a327f
Obtained from: https://github.com/GNOME/glib/commit/d18fcc9a5ad3791888f06e66b8d3c41210af2c68

--- gio/tests/pollable.c.orig
+++ gio/tests/pollable.c
@@ -28,6 +28,9 @@
 #ifdef HAVE_OPENPTY
 #include <pty.h>
 #endif
+#ifdef HAVE_UNISTD_H
+#include <unistd.h>
+#endif
 #include <gio/gunixinputstream.h>
 #include <gio/gunixoutputstream.h>
 #endif
@@ -185,6 +188,21 @@
     g_clear_object (&out);                                              \
   } G_STMT_END

+#define g_assert_pollable(fd) \
+  G_STMT_START {                                                        \
+    GPollableInputStream *in = NULL;                                    \
+    GOutputStream *out = NULL;                                          \
+                                                                        \
+    in = G_POLLABLE_INPUT_STREAM (g_unix_input_stream_new (fd, FALSE)); \
+    out = g_unix_output_stream_new (fd, FALSE);                         \
+                                                                        \
+    g_assert_true (g_pollable_input_stream_can_poll (in));              \
+    g_assert_true (g_pollable_output_stream_can_poll (G_POLLABLE_OUTPUT_STREAM (out))); \
+                                                                        \
+    g_clear_object (&in);                                               \
+    g_clear_object (&out);                                              \
+  } G_STMT_END
+
 static void
 test_pollable_unix_pipe (void)
 {
@@ -271,7 +289,17 @@
   int fd = g_open ("/dev/null", O_RDWR, 0);
   g_assert_cmpint (fd, !=, -1);

+#if defined(__FreeBSD__)
+  int freebsd_version = getosreldate ();
+  g_assert_cmpint (freebsd_version, !=, -1);
+  /* /dev/null is actually pollable on FreeBSD 14.4+ with both poll(2) and kevent(2) */
+  if (freebsd_version >= 1404000)
+    g_assert_pollable (fd);
+  else
+    g_assert_not_pollable (fd);
+#else
   g_assert_not_pollable (fd);
+#endif

   close (fd);
 #else
