--- tests/cli/harness.rs.orig
+++ tests/cli/harness.rs
@@ -22,7 +22,7 @@

 pub(super) fn managed_github_plugin_dir(config_home: &Path) -> PathBuf {
     config_home
-        .join("herdr-dev")
+        .join(app_dir_name())
         .join("plugins")
         .join("github-installations")
 }
