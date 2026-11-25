Backport upstream GLib FreeBSD test portability fixes.

Obtained from: https://github.com/GNOME/glib/commit/cec8230607d7f0324236812b4154042da56d637c

--- gio/tests/gdbus-auth.c.orig
+++ gio/tests/gdbus-auth.c
@@ -355,8 +355,11 @@
                                   sizeof (read_buffer),
                                   NULL,
                                   &local_error);
-  g_assert_error (local_error, G_IO_ERROR, G_IO_ERROR_CONNECTION_CLOSED);
-  g_assert_cmpint (read_len, <, 0);
+  if (read_len != 0)
+    {
+      g_assert_error (local_error, G_IO_ERROR, G_IO_ERROR_CONNECTION_CLOSED);
+      g_assert_cmpint (read_len, <, 0);
+    }
   g_clear_error (&local_error);

   write_buffer = g_strdup_printf ("AUTH\r\n");
