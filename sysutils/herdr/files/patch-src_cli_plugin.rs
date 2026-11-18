--- src/cli/plugin.rs.orig
+++ src/cli/plugin.rs
@@ -1727,6 +1727,8 @@
         PluginPlatform::Linux
     } else if cfg!(target_os = "macos") {
         PluginPlatform::Macos
+    } else if cfg!(target_os = "freebsd") {
+        PluginPlatform::Freebsd
     } else {
         PluginPlatform::Windows
     }
@@ -1737,6 +1739,7 @@
         PluginPlatform::Linux => "linux",
         PluginPlatform::Macos => "macos",
         PluginPlatform::Windows => "windows",
+        PluginPlatform::Freebsd => "freebsd",
     }
 }
 
