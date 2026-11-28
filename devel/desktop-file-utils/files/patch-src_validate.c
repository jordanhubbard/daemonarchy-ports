--- src/validate.c.orig
+++ src/validate.c
@@ -1966,6 +1966,10 @@
   interfaces = g_strsplit (value, ";", 0);
 
   for (i = 0; interfaces[i]; i++) {
+    /* Ignore the split field created by an optional final separator. */
+    if (interfaces[i][0] == '\0' && interfaces[i + 1] == NULL)
+      break;
+
     if (!g_dbus_is_interface_name (interfaces[i])) {
       print_fatal (kf, "value \"%s\" for key \"%s\" in group \"%s\" "
                        "contains an invalid interface name \"%s\"\n",
