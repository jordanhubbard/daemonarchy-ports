GObject dispose can run repeatedly. Clear the initialized mutex once at finalization.

--- gusb/gusb-context.c.orig
+++ gusb/gusb-context.c
@@ -140,11 +140,19 @@
 	g_clear_pointer(&priv->dict_replug, g_hash_table_unref);
 	g_clear_pointer(&priv->ctx, libusb_exit);
 	g_clear_pointer(&priv->idle_events, g_ptr_array_unref);
+
+	G_OBJECT_CLASS(g_usb_context_parent_class)->dispose(object);
+}
+
+static void
+g_usb_context_finalize(GObject *object)
+{
+	GUsbContextPrivate *priv = GET_PRIVATE(G_USB_CONTEXT(object));
+
 	g_mutex_clear(&priv->idle_events_mutex);
-
-	G_OBJECT_CLASS(g_usb_context_parent_class)->dispose(object);
-}
-
+	G_OBJECT_CLASS(g_usb_context_parent_class)->finalize(object);
+}
+
 static void
 g_usb_context_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
 {
@@ -191,6 +199,7 @@
 	GObjectClass *object_class = G_OBJECT_CLASS(klass);
 
 	object_class->dispose = g_usb_context_dispose;
+	object_class->finalize = g_usb_context_finalize;
 	object_class->get_property = g_usb_context_get_property;
 	object_class->set_property = g_usb_context_set_property;
 
