/*
 * Isolate packaged GLib from libfprint when investigating native failures.
 * This is a diagnostic control, not fingerprint or hardware acceptance.
 */
#include <gio/gio.h>
#include <glib/gstdio.h>

int
main (void)
{
  for (unsigned i = 0; i < 100; ++i)
    {
      GError *error = NULL;
      GCancellable *cancellable = g_cancellable_new ();
      char *directory = g_dir_make_tmp ("daemonarchy-glib-XXXXXX", &error);
      g_assert_no_error (error);
      g_assert_nonnull (directory);
      g_cancellable_cancel (cancellable);
      g_assert_true (g_cancellable_is_cancelled (cancellable));
      g_object_unref (cancellable);
      g_assert_cmpint (g_rmdir (directory), ==, 0);
      g_free (directory);
    }
  g_print ("GLib temporary-directory and cancellation control: 100 passed\n");
  return 0;
}
