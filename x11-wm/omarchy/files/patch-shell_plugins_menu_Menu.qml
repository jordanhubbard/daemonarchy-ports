--- shell/plugins/menu/Menu.qml.orig
+++ shell/plugins/menu/Menu.qml
@@ -41,6 +41,7 @@

   function refresh() {
     defaultMenuFile.reload()
+    vendorMenuFile.reload()
     userMenuFile.reload()
     languageFile.reload()
     menuCatalogFile.reload()
@@ -65,6 +66,10 @@
   readonly property string languagePath: Quickshell.env("OMARCHY_LANGUAGE_FILE") || ((Quickshell.env("XDG_CONFIG_HOME") || (Quickshell.env("HOME") + "/.config")) + "/omarchy/language")
   property var defaultMenuItems: []
   property var userMenuItems: []
+  // Package entries precede user overrides and retain upstream translations.
+  property string vendorMenuPath: "%%DATADIR%%/vendor/omarchy-menu.jsonc"
+  property var vendorMenuItems: []
+  property bool vendorMenuSettled: false
   // Whether each file has answered once, loaded or (the user's) missing. At
   // startup both arrive separately, and evaluating guards for the first alone
   // would run the whole batch twice; an open before then evaluates anyway.
@@ -298,14 +303,14 @@
     // "Go" label comes from the catalog's root entry; the merge only uses
     // it for the injected row, never over a user-supplied heading.
     var rootCatalog = root.menuCatalog.root
-    var mergedMenu = MenuModel.mergeMenuSources(translatedDefaults, root.userMenuItems, rootCatalog && rootCatalog.label)
+    var mergedMenu = MenuModel.mergeMenuSources(translatedDefaults.concat(root.vendorMenuItems), root.userMenuItems, rootCatalog && rootCatalog.label)
     root.providerRevision += 1
     root.providersLoaded = ({})
     root.providerQueue = []
     root.items = mergedMenu.items
     root.itemOrder = mergedMenu.itemOrder
     root.rowsLoaded = true
-    if (!skipGuards && root.defaultMenuSettled && root.userMenuSettled) root.evaluateGuards()
+    if (!skipGuards && root.defaultMenuSettled && root.vendorMenuSettled && root.userMenuSettled) root.evaluateGuards()
     if (root.opened) {
       root.rebuildDisplay()
       if (!root.dmenuActive) {
@@ -1153,7 +1158,17 @@
     printErrors: false
     onLoaded: { root.defaultMenuItems = MenuModel.keepMenuRows(root.defaultMenuItems, text(), true); root.defaultMenuSettled = true; root.rebuildItemsFromSources() }
     // A user file that loaded first held its evaluation for this one.
-    onLoadFailed: { root.defaultMenuSettled = true; if (root.userMenuSettled) root.evaluateGuards() }
+    onLoadFailed: { root.defaultMenuSettled = true; if (root.vendorMenuSettled && root.userMenuSettled) root.evaluateGuards() }
+    onFileChanged: reload()
+  }
+
+  FileView {
+    id: vendorMenuFile
+    path: root.vendorMenuPath
+    watchChanges: true
+    printErrors: false
+    onLoaded: { root.vendorMenuItems = MenuModel.keepMenuRows(root.vendorMenuItems, text(), false); root.vendorMenuSettled = true; root.rebuildItemsFromSources() }
+    onLoadFailed: { root.vendorMenuItems = []; root.vendorMenuSettled = true; root.rebuildItemsFromSources() }
     onFileChanged: reload()
   }

