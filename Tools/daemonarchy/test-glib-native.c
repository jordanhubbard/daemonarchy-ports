/*
 * Isolate packaged GLib from libfprint when investigating native failures.
 * This is a diagnostic control, not fingerprint or hardware acceptance.
 */
#include <gio/gio.h>
#include <glib/gstdio.h>
#include <gusb.h>
#include <string.h>

int
main (int argc, char **argv)
{
  gboolean usb = argc == 2 && strcmp (argv[1], "--usb") == 0;
  gboolean dispose = argc == 2 && strcmp (argv[1], "--usb-dispose") == 0;

  g_assert_true (argc == 1 || usb || dispose);
  for (unsigned i = 0; i < 100; ++i)
    {
      GError *error = NULL;
      if (usb || dispose)
        {
          GUsbContext *context = g_usb_context_new (&error);
          g_assert_no_error (error);
          g_assert_nonnull (context);
          g_usb_context_enumerate (context);
          if (dispose)
            g_object_run_dispose (G_OBJECT (context));
          g_object_unref (context);
        }
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
  g_print ("GLib/GUsb temporary-directory and cancellation control: 100 passed\n");
  return 0;
}
