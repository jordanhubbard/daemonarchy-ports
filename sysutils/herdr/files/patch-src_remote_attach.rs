--- src/remote/attach.rs.orig
+++ src/remote/attach.rs
@@ -291,6 +291,7 @@
         let os = match os.trim() {
             "Linux" => "linux",
             "Darwin" => "macos",
+            "FreeBSD" => "freebsd",
             _ => return None,
         };
         let arch = match arch.trim() {
@@ -308,6 +309,8 @@
             "macos"
         } else if cfg!(target_os = "windows") {
             "windows"
+        } else if cfg!(target_os = "freebsd") {
+            "freebsd"
         } else {
             "unknown"
         };
@@ -1704,6 +1707,8 @@
     emit "/usr/local/bin/herdr"
 "#,
         );
+    } else if platform.os == "freebsd" {
+        script.push_str("    emit \"/usr/local/bin/herdr\"\n");
     } else if platform.os == "linux" {
         script.push_str(
             r#"    emit "/home/linuxbrew/.linuxbrew/bin/herdr"
@@ -4490,7 +4495,39 @@
                 .asset_key(),
             "macos-aarch64"
         );
-        assert!(RemotePlatform::from_uname("FreeBSD", "x86_64").is_none());
+        for (uname_arch, arch) in [
+            ("amd64", "x86_64"),
+            ("x86_64", "x86_64"),
+            ("arm64", "aarch64"),
+            ("aarch64", "aarch64"),
+        ] {
+            assert_eq!(
+                RemotePlatform::from_uname("FreeBSD", uname_arch)
+                    .unwrap()
+                    .asset_key(),
+                format!("freebsd-{arch}")
+            );
+        }
+        assert!(RemotePlatform::from_uname("Plan9", "x86_64").is_none());
+        assert!(RemotePlatform::from_uname("FreeBSD", "unsupported").is_none());
+    }
+
+    #[test]
+    fn freebsd_remote_uses_pkg_discovery_and_posix_metadata() {
+        for arch in ["amd64", "arm64"] {
+            let platform = RemotePlatform::from_uname("FreeBSD", arch).unwrap();
+            assert!(!platform.is_windows());
+            assert!(known_remote_binary_candidate_script(&platform)
+                .contains("emit \"/usr/local/bin/herdr\""));
+            let remote =
+                RemoteHerdr::for_platform(platform).with_posix_path("/usr/local/bin/herdr");
+            let metadata = remote.machine_metadata().unwrap();
+            assert_eq!(metadata.os, "freebsd");
+            assert_eq!(metadata.executable, "/usr/local/bin/herdr");
+            assert!(metadata.is_valid());
+        }
+        #[cfg(target_os = "freebsd")]
+        assert_eq!(RemotePlatform::local().os, "freebsd");
     }

     #[test]
