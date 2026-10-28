--- t2-touchid/daemon/main.rs.orig	2026-10-28 20:04:07 UTC
+++ t2-touchid/daemon/main.rs
@@ -16,6 +16,7 @@ mod fprint;
 use cli::{Config, config};
 
 mod fprint;
+#[cfg(target_os = "linux")]
 mod keyboard;
 mod resume;
 mod signal;
@@ -75,11 +76,16 @@ fn main() -> Result<()> {
     let mut wait_for_fprintd_close = false;
     let verifying = Arc::new(AtomicBool::new(false));
     let cancel_requested = Arc::new(AtomicBool::new(false));
+    #[cfg(target_os = "linux")]
     if let Err(error) =
         keyboard::watch_escape(Arc::clone(&verifying), Arc::clone(&cancel_requested))
     {
         log(&format!("Escape key monitor unavailable: {error:#}"));
     }
+    // Elsewhere there is no Escape monitor: a prompt ends when the match
+    // window closes or the finger is recognised.
+    #[cfg(not(target_os = "linux"))]
+    log("Escape key monitor not available on this platform");
     let resumed = resume::watch();
     let mut unreachable_since: Option<Instant> = None;
     log(&format!(
