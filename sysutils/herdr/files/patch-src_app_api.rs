--- src/app/api.rs.orig
+++ src/app/api.rs
@@ -1573,6 +1573,7 @@
 
     #[tokio::test]
     async fn server_reload_agent_manifests_resets_detection_runtimes() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let (_api_tx, api_rx) = tokio::sync::mpsc::unbounded_channel();
         let mut app = App::new(
             &crate::config::Config::default(),
@@ -1630,6 +1631,7 @@
 
     #[tokio::test]
     async fn server_agent_manifests_reports_status_without_resetting_runtimes() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let (_api_tx, api_rx) = tokio::sync::mpsc::unbounded_channel();
         let mut app = App::new(
             &crate::config::Config::default(),
@@ -1673,6 +1675,7 @@
 
     #[tokio::test]
     async fn agent_explain_evaluates_with_server_manifest_cache() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let (_api_tx, api_rx) = tokio::sync::mpsc::unbounded_channel();
         let mut app = App::new(
             &crate::config::Config::default(),
@@ -1718,6 +1721,7 @@
 
     #[tokio::test]
     async fn agent_explain_rejects_hook_only_full_lifecycle_authority() {
+        let _config_env_guard = crate::config::test_config_env_lock().lock().unwrap();
         let (_api_tx, api_rx) = tokio::sync::mpsc::unbounded_channel();
         let mut app = App::new(
             &crate::config::Config::default(),
