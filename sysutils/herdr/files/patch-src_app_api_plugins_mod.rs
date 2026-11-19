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
@@ -1941,7 +1941,7 @@
 name = "Pane Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2048,7 +2048,7 @@
 name = "Path Env"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2152,7 +2152,7 @@
 name = "Tab Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2235,7 +2235,7 @@
 name = "Split Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2314,7 +2314,7 @@
 name = "Overlay Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2395,7 +2395,7 @@
 name = "Popup Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2508,6 +2508,7 @@
                 crate::api::schema::PluginPlatform::Linux,
                 crate::api::schema::PluginPlatform::Macos,
                 crate::api::schema::PluginPlatform::Windows,
+                crate::api::schema::PluginPlatform::Freebsd,
             ])
         );
 
@@ -2813,7 +2814,7 @@
 name = "Runner"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[actions]]
 id = "run"
@@ -2880,7 +2881,7 @@
 name = "Action Paths"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[actions]]
 id = "run"
@@ -2991,7 +2992,7 @@
 name = "Startup"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[startup]]
 command = ["sh", "-c", "printf '%s:%s' \"$HERDR_PLUGIN_ID\" \"$HERDR_PLUGIN_EVENT\" > {}"]
@@ -3038,7 +3039,7 @@
 name = "Event Context"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[events]]
 on = "worktree.created"
@@ -3256,7 +3257,7 @@
 name = "Links"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[actions]]
 id = "open"
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
