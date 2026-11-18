--- src/app/api/plugins/manifest.rs.orig
+++ src/app/api/plugins/manifest.rs
@@ -108,6 +108,7 @@
             "linux" => Ok(RawPlatform(PluginPlatform::Linux)),
             "macos" => Ok(RawPlatform(PluginPlatform::Macos)),
             "windows" => Ok(RawPlatform(PluginPlatform::Windows)),
+            "freebsd" => Ok(RawPlatform(PluginPlatform::Freebsd)),
             other => Err(format!(
                 "invalid_plugin_platform: unknown platform '{other}'"
             )),
@@ -519,6 +520,8 @@
         PluginPlatform::Linux
     } else if cfg!(target_os = "macos") {
         PluginPlatform::Macos
+    } else if cfg!(target_os = "freebsd") {
+        PluginPlatform::Freebsd
     } else {
         PluginPlatform::Windows
     }
@@ -562,6 +565,7 @@
         PluginPlatform::Linux => "linux",
         PluginPlatform::Macos => "macos",
         PluginPlatform::Windows => "windows",
+        PluginPlatform::Freebsd => "freebsd",
     }
 }
 
@@ -615,3 +619,25 @@
             .all(|byte| byte.is_ascii_alphanumeric() || matches!(byte, b':' | b'_' | b'-')))
     .then(|| value.to_string())
 }
+
+#[cfg(test)]
+mod freebsd_platform_tests {
+    use super::*;
+
+    #[test]
+    fn declared_freebsd_platform_parses_and_serializes() {
+        let platform = RawPlatform::try_from("freebsd".to_string()).unwrap().0;
+        assert_eq!(platform, PluginPlatform::Freebsd);
+        assert_eq!(serde_json::to_string(&platform).unwrap(), "\"freebsd\"");
+        assert_eq!(platform_name(platform), "freebsd");
+    }
+
+    #[cfg(target_os = "freebsd")]
+    #[test]
+    fn current_freebsd_host_enforces_explicit_platform_declarations() {
+        assert_eq!(current_platform(), PluginPlatform::Freebsd);
+        assert!(ensure_platform_supported(&Some(vec![PluginPlatform::Freebsd]), "native").is_ok());
+        assert!(ensure_platform_supported(&Some(vec![PluginPlatform::Linux]), "linux-only").is_err());
+        assert!(ensure_platform_supported(&None, "undeclared").is_ok());
+    }
+}
