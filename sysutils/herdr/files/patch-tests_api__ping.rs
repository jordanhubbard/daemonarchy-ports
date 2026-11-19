--- tests/api_ping.rs.orig
+++ tests/api_ping.rs
@@ -364,7 +364,12 @@
     let child = spawn_herdr(&config_home, &runtime_dir, &socket_path);
     wait_for_socket(&socket_path, Duration::from_secs(5));

-    let override_dir = config_home.join("herdr-dev").join("agent-detection");
+    let app_dir = if cfg!(debug_assertions) {
+        "herdr-dev"
+    } else {
+        "herdr"
+    };
+    let override_dir = config_home.join(app_dir).join("agent-detection");
     fs::create_dir_all(&override_dir).unwrap();
     let override_path = override_dir.join("codex.toml");
     fs::write(
