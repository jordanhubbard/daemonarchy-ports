--- t2-touchid/daemon/fprint.rs.orig	2026-10-28 20:04:07 UTC
+++ t2-touchid/daemon/fprint.rs
@@ -16,6 +16,7 @@ pub const STORE: &str = "/var/lib/fprint";
 
 /// Check whether libfprint is listening without opening a client connection.
 /// Every connection replaces the current client and must carry a command.
+#[cfg(target_os = "linux")]
 pub fn device_is_open(path: &str) -> bool {
     let Ok(sockets) = std::fs::read_to_string("/proc/net/unix") else {
         return false;
@@ -23,6 +24,29 @@ pub fn device_is_open(path: &str) -> bool {
     socket_table_contains(&sockets, path)
 }
 
+/// FreeBSD has no /proc/net/unix; sockstat(1) lists the bound Unix sockets,
+/// with -w so that long paths are not cut short.
+#[cfg(target_os = "freebsd")]
+pub fn device_is_open(path: &str) -> bool {
+    let Ok(output) = std::process::Command::new("/usr/bin/sockstat")
+        .args(["-l", "-u", "-q", "-w"])
+        .stderr(std::process::Stdio::null())
+        .output()
+    else {
+        return false;
+    };
+    output.status.success() && sockstat_lists(&String::from_utf8_lossy(&output.stdout), path)
+}
+
+#[cfg(target_os = "freebsd")]
+fn sockstat_lists(sockets: &str, path: &str) -> bool {
+    // USER COMMAND PID FD PROTO LOCAL FOREIGN
+    sockets
+        .lines()
+        .any(|line| line.split_whitespace().nth(5) == Some(path))
+}
+
+#[cfg_attr(not(target_os = "linux"), allow(dead_code))]
 fn socket_table_contains(sockets: &str, path: &str) -> bool {
     sockets
         .lines()
@@ -90,5 +114,15 @@ mod tests {
 
         assert!(socket_table_contains(sockets, "/run/t2-touchid/fprint.sock"));
         assert!(!socket_table_contains(sockets, "/run/other.sock"));
+    }
+
+    #[cfg(target_os = "freebsd")]
+    #[test]
+    fn matches_socket_path_in_sockstat() {
+        let sockets = "root     fprintd     1234 7  stream  /var/run/t2-touchid/fprint.sock ??\n\
+                       root     sshd-sessi  88 10 stream  (not connected)                 ??\n";
+
+        assert!(super::sockstat_lists(sockets, "/var/run/t2-touchid/fprint.sock"));
+        assert!(!super::sockstat_lists(sockets, "/var/run/other.sock"));
     }
 }
