--- tests/test-fpi-device.c.orig
+++ tests/test-fpi-device.c
@@ -17,7 +17,10 @@
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
  */
 
+#include <math.h>
+
 #include "fp-device.h"
+#include "fp-device-private.h"
 #include "fp-enums.h"
 #include <libfprint/fprint.h>
 
@@ -2330,7 +2333,7 @@
 }
 
 static void
-test_driver_identify_warmup_cooldown (void)
+test_driver_identify_warmup_cooldown (gconstpointer user_data)
 {
   g_autoptr(FpAutoResetClass) dev_class = auto_reset_device_class ();
   g_autoptr(MatchCbData) identify_data = g_new0 (MatchCbData, 1);
@@ -2340,6 +2343,10 @@
   void (*orig_identify) (FpDevice *device);
   FpiDeviceFake *fake_dev;
   gint64 start_time;
+  gint64 heating_start;
+  gdouble cooling_ratio;
+  gdouble warm_deadline;
+  gdouble cold_deadline;
 
   dev_class->temp_hot_seconds = 2;
   dev_class->temp_cold_seconds = 5;
@@ -2355,6 +2362,8 @@
   fake_dev->last_called_function = NULL;
 
   fake_dev->ret_error = fpi_device_error_new (FP_DEVICE_ERROR_GENERAL);
+
+  heating_start = g_get_monotonic_time ();
 
   /* Undefined: Whether match_cb is called. */
   fp_device_identify (device, prints, NULL,
@@ -2385,6 +2394,8 @@
     continue;
   g_assert (fake_dev->last_called_function == dev_class->cancel);
 
+  /* Exercise delayed cancellation without changing the thermal model. */
+  g_usleep (GPOINTER_TO_UINT (user_data));
   orig_identify (device);
   fake_dev->ret_error = NULL;
   while (g_main_context_iteration (NULL, FALSE))
@@ -2397,19 +2408,34 @@
   g_assert_error (error, FP_DEVICE_ERROR, FP_DEVICE_ERROR_TOO_HOT);
   g_clear_error (&error);
 
-  /* Now, wait for it to cool down again;
-   * WARM should be reached after about 2s
-   * COLD after 5s but give it some more slack. */
+  /* The device keeps heating until identify completes. A delayed HOT
+   * callback or cancellation therefore increases its cooling time. Bound
+   * the starting ratio by heating from COLD for the measured full interval:
+   *   r <= 1 - (1 - COLD) * exp(-elapsed / hot_seconds).
+   * Cooling to a threshold takes cold_seconds * log(r / threshold).
+   * Keep the model's 100ms delay and the original 150ms/400ms scheduler slack.
+   * The original warm-up deadline above remains fixed and enforced.
+   */
+  cooling_ratio = 1.0 - (1.0 - TEMP_COLD_THRESH) *
+    exp (-(g_get_monotonic_time () - heating_start) /
+         (1e6 * dev_class->temp_hot_seconds));
+  warm_deadline = dev_class->temp_cold_seconds *
+    log (cooling_ratio / TEMP_HOT_WARM_THRESH) + TEMP_DELAY_SECONDS + 0.15;
+  cold_deadline = dev_class->temp_cold_seconds *
+    log (cooling_ratio / TEMP_COLD_THRESH) + TEMP_DELAY_SECONDS + 0.4;
+  g_test_message ("Cooling ratio bound %.6f, WARM deadline %.6fs, COLD %.6fs",
+                  cooling_ratio, warm_deadline, cold_deadline);
+
   start_time = g_get_monotonic_time ();
   while (fp_device_get_temperature (device) == FP_TEMPERATURE_HOT)
     g_main_context_iteration (NULL, TRUE);
   g_assert_cmpint (fp_device_get_temperature (device), ==, FP_TEMPERATURE_WARM);
-  g_assert_cmpint (g_get_monotonic_time () - start_time, <, 2000000 + 250000);
+  g_assert_cmpfloat ((g_get_monotonic_time () - start_time) / 1e6, <, warm_deadline);
 
   while (fp_device_get_temperature (device) == FP_TEMPERATURE_WARM)
     g_main_context_iteration (NULL, TRUE);
   g_assert_cmpint (fp_device_get_temperature (device), ==, FP_TEMPERATURE_COLD);
-  g_assert_cmpint (g_get_monotonic_time () - start_time, <, 5000000 + 500000);
+  g_assert_cmpfloat ((g_get_monotonic_time () - start_time) / 1e6, <, cold_deadline);
 }
 
 static void
@@ -3466,7 +3492,11 @@
   g_test_add_func ("/driver/identify/suspend_busy_error", test_driver_identify_suspend_busy_error);
   g_test_add_func ("/driver/identify/suspend_while_idle", test_driver_identify_suspend_while_idle);
 
-  g_test_add_func ("/driver/identify/warmup_cooldown", test_driver_identify_warmup_cooldown);
+  g_test_add_data_func ("/driver/identify/warmup_cooldown", NULL,
+                        test_driver_identify_warmup_cooldown);
+  g_test_add_data_func ("/driver/identify/warmup_cooldown_delayed_cancel",
+                        GUINT_TO_POINTER (250000),
+                        test_driver_identify_warmup_cooldown);
 
   g_test_add_func ("/driver/capture", test_driver_capture);
   g_test_add_func ("/driver/capture/not_supported", test_driver_capture_not_supported);
