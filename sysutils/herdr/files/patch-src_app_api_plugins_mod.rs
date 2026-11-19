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
@@ -1913,6 +1913,7 @@
     #[cfg(unix)]
     #[tokio::test]
     async fn plugin_pane_open_uses_plugin_root_title_env_and_target_context() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let mut app = test_app();
         let mut workspace = crate::workspace::Workspace::test_new("plugin-target");
         workspace.custom_name = None;
@@ -1941,7 +1942,7 @@
 name = "Pane Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2032,6 +2033,7 @@
     #[cfg(unix)]
     #[tokio::test]
     async fn plugin_pane_open_injects_plugin_paths_and_protects_overrides() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let mut app = test_app();
         app.state.workspaces = vec![crate::workspace::Workspace::test_new("plugin-path-env")];
         app.state.ensure_test_terminals();
@@ -2048,7 +2050,7 @@
 name = "Path Env"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2129,6 +2131,7 @@
     #[cfg(unix)]
     #[tokio::test]
     async fn plugin_pane_open_tab_emits_tab_created_before_pane_created() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let event_hub = crate::api::EventHub::default();
         let (_api_tx, api_rx) = tokio::sync::mpsc::unbounded_channel();
         let mut app = App::new(
@@ -2152,7 +2155,7 @@
 name = "Tab Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2212,6 +2215,7 @@
     #[cfg(unix)]
     #[tokio::test]
     async fn plugin_pane_open_zoomed_split_emits_layout_updated() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let event_hub = crate::api::EventHub::default();
         let (_api_tx, api_rx) = tokio::sync::mpsc::unbounded_channel();
         let mut app = App::new(
@@ -2235,7 +2239,7 @@
 name = "Split Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2291,6 +2295,7 @@
     #[cfg(unix)]
     #[tokio::test]
     async fn plugin_pane_open_overlay_emits_layout_updated() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let event_hub = crate::api::EventHub::default();
         let (_api_tx, api_rx) = tokio::sync::mpsc::unbounded_channel();
         let mut app = App::new(
@@ -2314,7 +2319,7 @@
 name = "Overlay Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2370,6 +2375,7 @@
     #[cfg(unix)]
     #[tokio::test]
     async fn plugin_pane_open_popup_is_layout_neutral() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let event_hub = crate::api::EventHub::default();
         let (_api_tx, api_rx) = tokio::sync::mpsc::unbounded_channel();
         let mut app = App::new(
@@ -2395,7 +2401,7 @@
 name = "Popup Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[panes]]
 id = "board"
@@ -2508,6 +2514,7 @@
                 crate::api::schema::PluginPlatform::Linux,
                 crate::api::schema::PluginPlatform::Macos,
                 crate::api::schema::PluginPlatform::Windows,
+                crate::api::schema::PluginPlatform::Freebsd,
             ])
         );
 
@@ -2804,6 +2811,7 @@
     #[cfg(unix)]
     #[test]
     fn manifest_action_invoke_runs_command_and_captures_log() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let mut app = test_app();
         let root = unique_temp_path("plugin-action-runner");
         write_manifest_content(
@@ -2813,7 +2821,7 @@
 name = "Runner"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[actions]]
 id = "run"
@@ -2871,6 +2879,7 @@
     #[cfg(unix)]
     #[test]
     fn manifest_action_invoke_injects_plugin_paths() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let mut app = test_app();
         let root = unique_temp_path("plugin-action-path-env");
         write_manifest_content(
@@ -2880,7 +2889,7 @@
 name = "Action Paths"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[actions]]
 id = "run"
@@ -2980,6 +2989,7 @@
     #[cfg(unix)]
     #[test]
     fn startup_hooks_run_once_with_plugin_environment() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let mut app = test_app();
         let root = unique_temp_path("plugin-startup-hook");
         let capture = root.join("startup.txt");
@@ -2991,7 +3001,7 @@
 name = "Startup"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[startup]]
 command = ["sh", "-c", "printf '%s:%s' \"$HERDR_PLUGIN_ID\" \"$HERDR_PLUGIN_EVENT\" > {}"]
@@ -3017,6 +3027,7 @@
     #[cfg(unix)]
     #[test]
     fn event_hooks_use_event_target_context() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let mut app = test_app();
         app.state.workspaces = vec![
             crate::workspace::Workspace::test_new("active"),
@@ -3038,7 +3049,7 @@
 name = "Event Context"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[events]]
 on = "worktree.created"
@@ -3242,6 +3253,7 @@
     #[cfg(unix)]
     #[test]
     fn plugin_link_handler_invokes_action_with_clicked_url_context() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let mut app = test_app();
         app.state.workspaces = vec![crate::workspace::Workspace::test_new("link-handler")];
         app.state.ensure_test_terminals();
@@ -3256,7 +3268,7 @@
 name = "Links"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos"]
+platforms = ["linux", "macos", "freebsd"]
 
 [[actions]]
 id = "open"
@@ -3324,7 +3336,7 @@
 name = "Link Order"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "specific"
@@ -3371,7 +3383,7 @@
 name = "Bad Links"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "open"
@@ -3414,7 +3426,7 @@
 name = "Bad Link Action"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "open"
@@ -4020,7 +4032,7 @@
 name = "Override"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 
 [[actions]]
 id = "act"
