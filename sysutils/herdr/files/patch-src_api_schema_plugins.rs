--- src/api/schema/plugins.rs.orig
+++ src/api/schema/plugins.rs
@@ -348,6 +348,7 @@
     Linux,
     Macos,
     Windows,
+    Freebsd,
 }
 
 #[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize, schemars::JsonSchema)]
