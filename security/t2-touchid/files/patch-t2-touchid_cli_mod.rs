--- t2-touchid/cli/mod.rs.orig	2026-10-28 20:04:07 UTC
+++ t2-touchid/cli/mod.rs
@@ -11,7 +11,11 @@ pub fn config() -> Result<Config> {
 }
 
 pub fn config() -> Result<Config> {
-    let mut socket = String::from("/run/t2-touchid/fprint.sock");
+    let mut socket = String::from(if cfg!(target_os = "linux") {
+        "/run/t2-touchid/fprint.sock"
+    } else {
+        "/var/run/t2-touchid/fprint.sock"
+    });
     let mut user_id = 501;
     let mut flags = 0;
     let mut bind_user = None;
