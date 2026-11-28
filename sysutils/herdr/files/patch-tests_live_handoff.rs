--- tests/live_handoff.rs.orig
+++ tests/live_handoff.rs
@@ -51,6 +51,14 @@
     PathBuf::from(format!("/tmp/hlh-{}-{n}", std::process::id()))
 }

+fn app_dir_name() -> &'static str {
+    if cfg!(debug_assertions) {
+        "herdr-dev"
+    } else {
+        "herdr"
+    }
+}
+
 fn spawn_server(config_home: &Path, runtime_dir: &Path, api_socket: &Path) -> SpawnedHerdr {
     spawn_server_with_env(config_home, runtime_dir, api_socket, &[])
 }
@@ -61,10 +69,10 @@
     api_socket: &Path,
     extra_env: &[(&str, &str)],
 ) -> SpawnedHerdr {
-    fs::create_dir_all(config_home.join("herdr")).unwrap();
+    fs::create_dir_all(config_home.join(app_dir_name())).unwrap();
     fs::create_dir_all(runtime_dir).unwrap();
     fs::write(
-        config_home.join("herdr/config.toml"),
+        config_home.join(app_dir_name()).join("config.toml"),
         "onboarding = false\n",
     )
     .unwrap();
@@ -105,10 +113,10 @@
     runtime_dir: &Path,
     session_name: &str,
 ) -> SpawnedHerdr {
-    fs::create_dir_all(config_home.join("herdr-dev")).unwrap();
+    fs::create_dir_all(config_home.join(app_dir_name())).unwrap();
     fs::create_dir_all(runtime_dir).unwrap();
     fs::write(
-        config_home.join("herdr-dev/config.toml"),
+        config_home.join(app_dir_name()).join("config.toml"),
         "onboarding = false\n",
     )
     .unwrap();
@@ -140,10 +148,10 @@
 }

 fn spawn_default_session_server(config_home: &Path, runtime_dir: &Path) -> SpawnedHerdr {
-    fs::create_dir_all(config_home.join("herdr-dev")).unwrap();
+    fs::create_dir_all(config_home.join(app_dir_name())).unwrap();
     fs::create_dir_all(runtime_dir).unwrap();
     fs::write(
-        config_home.join("herdr-dev/config.toml"),
+        config_home.join(app_dir_name()).join("config.toml"),
         "onboarding = false\n",
     )
     .unwrap();
@@ -182,10 +190,10 @@
     api_socket_env: Option<&Path>,
     client_socket_env: Option<&Path>,
 ) -> SpawnedHerdr {
-    fs::create_dir_all(config_home.join("herdr-dev")).unwrap();
+    fs::create_dir_all(config_home.join(app_dir_name())).unwrap();
     fs::create_dir_all(runtime_dir).unwrap();
     fs::write(
-        config_home.join("herdr-dev/config.toml"),
+        config_home.join(app_dir_name()).join("config.toml"),
         "onboarding = false\n",
     )
     .unwrap();
@@ -319,7 +327,7 @@
 name = "Live handoff test"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 "#
         ),
     )
@@ -428,6 +436,7 @@

 #[test]
 fn pid_marker_waits_for_complete_line() {
+    let _lock = test_lock();
     let base = unique_test_dir();
     fs::create_dir_all(&base).unwrap();
     let marker = base.join("child.pid");
@@ -736,7 +745,7 @@
     }

     let session: serde_json::Value = serde_json::from_slice(
-        &fs::read(config_home.join("herdr-dev/session.json")).expect("saved session"),
+        &fs::read(config_home.join(app_dir_name()).join("session.json")).expect("saved session"),
     )
     .expect("valid session json");
     assert_eq!(session["workspaces"].as_array().map(Vec::len), Some(1));
@@ -829,7 +838,7 @@
     let base = unique_test_dir();
     let config_home = base.join("config");
     let runtime_dir = base.join("runtime");
-    let session_dir = config_home.join("herdr-dev/sessions/work");
+    let session_dir = config_home.join(app_dir_name()).join("sessions/work");
     let api_socket = session_dir.join("herdr.sock");
     let client_socket = session_dir.join("herdr-client.sock");

@@ -845,7 +854,7 @@
     wait_for_api(&api_socket, Duration::from_secs(10));
     wait_for_socket(&client_socket, Duration::from_secs(5));
     assert!(
-        !config_home.join("herdr-dev/herdr.sock").exists(),
+        !config_home.join(app_dir_name()).join("herdr.sock").exists(),
         "named handoff unexpectedly bound the default session API socket"
     );

@@ -862,10 +871,10 @@
     let base = unique_test_dir();
     let config_home = base.join("config");
     let runtime_dir = base.join("runtime");
-    let default_session_dir = config_home.join("herdr-dev");
+    let default_session_dir = config_home.join(app_dir_name());
     let default_api_socket = default_session_dir.join("herdr.sock");
     let default_client_socket = default_session_dir.join("herdr-client.sock");
-    let work_session_dir = config_home.join("herdr-dev/sessions/work");
+    let work_session_dir = config_home.join(app_dir_name()).join("sessions/work");
     let work_api_socket = work_session_dir.join("herdr.sock");
     let work_client_socket = work_session_dir.join("herdr-client.sock");

@@ -909,7 +918,7 @@
     let base = unique_test_dir();
     let config_home = base.join("config");
     let runtime_dir = base.join("runtime");
-    let api_socket = config_home.join("herdr-dev/herdr.sock");
+    let api_socket = config_home.join(app_dir_name()).join("herdr.sock");
     let client_socket = runtime_dir.join("custom-client.sock");

     let spawned = spawn_server_with_args_and_socket_env(
@@ -944,8 +953,8 @@
     let base = unique_test_dir();
     let config_home = base.join("config");
     let runtime_dir = base.join("runtime");
-    let api_socket = config_home.join("herdr-dev/herdr.sock");
-    let registry_path = config_home.join("herdr-dev/plugins.json");
+    let api_socket = config_home.join(app_dir_name()).join("herdr.sock");
+    let registry_path = config_home.join(app_dir_name()).join("plugins.json");
     let existing_plugin = base.join("plugins/existing");
     let added_plugin = base.join("plugins/added");
     write_plugin_manifest(&existing_plugin, "test.live-handoff-existing");
@@ -1869,10 +1878,10 @@
     let config_home = base.join("config");
     let runtime_dir = base.join("runtime");
     let sessions = [
-        (None, config_home.join("herdr-dev/herdr.sock")),
+        (None, config_home.join(app_dir_name()).join("herdr.sock")),
         (
             Some("work"),
-            config_home.join("herdr-dev/sessions/work/herdr.sock"),
+            config_home.join(app_dir_name()).join("sessions/work/herdr.sock"),
         ),
     ];
     let mut spawned = Vec::new();
