--- src/integration/env.rs.orig
+++ src/integration/env.rs
@@ -1,7 +1,7 @@
 use std::io;
 use std::path::PathBuf;
 #[cfg(test)]
-use std::sync::{Mutex, MutexGuard, OnceLock};
+use std::sync::MutexGuard;
 
 use portable_pty::CommandBuilder;
 
@@ -234,6 +234,7 @@
 #[cfg(test)]
 pub(crate) struct IntegrationEnvLock {
     _guard: MutexGuard<'static, ()>,
+    home: Option<std::ffi::OsString>,
     #[cfg(windows)]
     appdata: Option<std::ffi::OsString>,
 }
@@ -241,6 +242,11 @@
 #[cfg(test)]
 impl Drop for IntegrationEnvLock {
     fn drop(&mut self) {
+        if let Some(home) = self.home.take() {
+            std::env::set_var("HOME", home);
+        } else {
+            std::env::remove_var("HOME");
+        }
         #[cfg(windows)]
         if let Some(appdata) = self.appdata.take() {
             std::env::set_var("APPDATA", appdata);
@@ -252,10 +258,10 @@
 
 #[cfg(test)]
 pub(crate) fn integration_env_lock() -> IntegrationEnvLock {
-    static LOCK: OnceLock<Mutex<()>> = OnceLock::new();
-    let guard = LOCK.get_or_init(|| Mutex::new(())).lock().unwrap();
+    let guard = crate::config::test_config_env_lock().lock().unwrap();
     IntegrationEnvLock {
         _guard: guard,
+        home: std::env::var_os("HOME"),
         #[cfg(windows)]
         appdata: std::env::var_os("APPDATA"),
     }
