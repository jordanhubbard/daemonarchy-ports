--- shell/plugins/bar/widgets/SystemUpdate.qml.orig
+++ shell/plugins/bar/widgets/SystemUpdate.qml
@@ -58,7 +58,7 @@
     bar: root.bar
     text: "\uf021"
     slotSize: Style.bar.statusSlot
-    tooltipText: I18n.t("bar.pendingUpdates")
+    tooltipText: I18n.t("bar.pendingUpdates").replace(/Omarchy/g, Quickshell.env("OMARCHY_DISTRIBUTION_NAME") || "Omarchy")
     onPressed: root.runUpdate()
   }
 }
