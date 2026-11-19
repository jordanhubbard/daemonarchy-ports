--- src/client/endpoint/ssh_metadata.rs.orig
+++ src/client/endpoint/ssh_metadata.rs
@@ -20,7 +20,9 @@
             return false;
         }
         match self.os.as_str() {
-            "linux" | "macos" => path.starts_with('/') && !path.ends_with("/mise/shims/herdr"),
+            "linux" | "macos" | "freebsd" => {
+                path.starts_with('/') && !path.ends_with("/mise/shims/herdr")
+            }
             "windows" => {
                 let bytes = path.as_bytes();
                 path.starts_with(r"\\")
@@ -125,6 +127,10 @@
         for (os, path, valid) in [
             ("linux", "/home/a b/herdr", true),
             ("macos", "/opt/homebrew/bin/herdr", true),
+            ("freebsd", "/usr/local/bin/herdr", true),
+            ("freebsd", "herdr", false),
+            ("freebsd", "/home/user/.local/share/mise/shims/herdr", false),
+            ("freebsd", "/bin/herdr\nmalformed", false),
             ("windows", r"C:\Program Files\herdr.exe", true),
             ("windows", r"\\server\share\herdr.exe", true),
             ("windows", "herdr.exe", false),
