--- shared/protocols/t2-bridgexpc/src/remote.rs.orig	2026-10-28 20:04:07 UTC
+++ shared/protocols/t2-bridgexpc/src/remote.rs
@@ -71,6 +71,9 @@ impl Connection {
         let index = unsafe { libc::if_nametoindex(std::ffi::CString::new(interface)?.as_ptr()) };
         ensure!(index != 0, "unknown interface {interface}");
         let socket = Socket::new(Domain::IPV6, Type::STREAM, Some(Protocol::TCP))?;
+        // Elsewhere the link-local scope id below already ties the
+        // connection to the CDC-NCM interface.
+        #[cfg(target_os = "linux")]
         socket
             .bind_device(Some(interface.as_bytes()))
             .context("bind socket to CDC-NCM interface")?;
