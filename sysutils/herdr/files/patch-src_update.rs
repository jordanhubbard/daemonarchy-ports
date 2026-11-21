--- src/update.rs.orig
+++ src/update.rs
@@ -2109,6 +2109,13 @@
 pub fn self_update(options: SelfUpdateOptions) -> Result<Version, String> {
     let channel = UpdateChannel::configured();
 
+    // Herdr publishes no FreeBSD binaries; the package manager owns the install.
+    if cfg!(target_os = "freebsd") {
+        return Err(
+            "self-update is disabled for FreeBSD package installs; run `pkg upgrade herdr`".into(),
+        );
+    }
+
     if is_homebrew_managed_install() {
         if channel == UpdateChannel::Preview {
             return Err(
@@ -2346,6 +2353,10 @@
 /// Background update check: only surface availability and release notes.
 /// Runs in a background thread at startup.
 pub fn auto_update(events: tokio::sync::mpsc::Sender<crate::events::AppEvent>) {
+    // Herdr publishes no FreeBSD binaries; updates come from pkg(8).
+    if cfg!(target_os = "freebsd") {
+        return;
+    }
     crate::logging::update_check_started();
     if let Ok(version) = env::var(FAKE_UPDATE_VERSION_ENV) {
         let version = version.trim();
@@ -2481,6 +2492,8 @@
         "macos"
     } else if cfg!(target_os = "windows") {
         "windows"
+    } else if cfg!(target_os = "freebsd") {
+        "freebsd"
     } else {
         "unknown"
     };
@@ -3612,7 +3625,7 @@
     #[test]
     fn platform_target_is_known() {
         let (os, arch) = platform_target();
-        assert!(os == "linux" || os == "macos", "os: {os}");
+        assert_eq!(os, std::env::consts::OS);
         assert!(arch == "x86_64" || arch == "aarch64", "arch: {arch}");
     }
 
