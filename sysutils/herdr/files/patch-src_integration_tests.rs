--- src/integration/tests.rs.orig
+++ src/integration/tests.rs
@@ -4238,6 +4238,7 @@
 
 #[test]
 fn letta_staged_install_can_restore_the_prior_file() {
+    let _lock = integration_env_lock();
     let base = unique_base();
     fs::create_dir_all(&base).unwrap();
     let target = base.join("settings.json");
