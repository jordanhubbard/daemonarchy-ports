--- src/app/api/plugins/mod.rs.orig
+++ src/app/api/plugins/mod.rs
@@ -902,7 +902,7 @@
 name = "Failed link"
 version = "0.1.0"
 min_herdr_version = "0.9.3"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 [[actions]]
 id = "open"
 title = "Open note"
@@ -1007,7 +1007,7 @@
 version = "0.1.0"
 min_herdr_version = "0.6.10"
 description = "Prepare new worktrees"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[build]]
 command = ["bun", "install"]
@@ -1075,7 +1075,7 @@
 name = "Config Dirs"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 "#,
         );
 
@@ -1110,7 +1110,7 @@
 name = "Legacy Config"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 "#,
         );
 
@@ -1341,7 +1341,7 @@
 id = "example.missing-min-herdr"
 name = "Missing Min Herdr"
 version = "0.1.0"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 "#,
                 "invalid_plugin_min_herdr_version",
             ),
@@ -1352,7 +1352,7 @@
 name = "Invalid Min Herdr"
 version = "0.1.0"
 min_herdr_version = "soon"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 "#,
                 "invalid_plugin_min_herdr_version",
             ),
@@ -1363,7 +1363,7 @@
 name = "Future Min Herdr"
 version = "0.1.0"
 min_herdr_version = "999.0.0"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 "#,
                 "plugin_requires_newer_herdr",
             ),
@@ -1374,7 +1374,7 @@
 name = "Non Popup Size"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -1410,7 +1410,7 @@
 name = "Duplicate"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "run"
@@ -1439,7 +1439,7 @@
 name = "Dotted Action"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "build.release"
@@ -1463,7 +1463,7 @@
 name = "Duplicate Pane"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[panes]]
 id = "ui"
@@ -1492,7 +1492,7 @@
 name = "Startup Manifest"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[startup]]
 command = ["node", "restore.js"]
@@ -2508,6 +2508,7 @@
                 crate::api::schema::PluginPlatform::Linux,
                 crate::api::schema::PluginPlatform::Macos,
                 crate::api::schema::PluginPlatform::Windows,
+                crate::api::schema::PluginPlatform::Freebsd,
             ])
         );
 
@@ -3324,7 +3325,7 @@
 name = "Link Order"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "specific"
@@ -3371,7 +3372,7 @@
 name = "Bad Links"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "open"
@@ -3414,7 +3415,7 @@
 name = "Bad Link Action"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "open"
@@ -4020,7 +4021,7 @@
 name = "Override"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "act"
