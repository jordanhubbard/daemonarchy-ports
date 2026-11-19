--- tests/cli/plugins.rs.orig
+++ tests/cli/plugins.rs
@@ -126,7 +126,7 @@
 name = "Offline Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 "#,
     )
     .unwrap();
@@ -185,7 +185,7 @@
 name = "Global Plugin"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]
 "#,
     )
     .unwrap();
@@ -670,7 +670,7 @@
     );

     // Model an existing installation from before generation directories.
-    let registry = config_home.join("herdr-dev/plugins.json");
+    let registry = config_home.join(app_dir_name()).join("plugins.json");
     let mut entries: serde_json::Value =
         serde_json::from_slice(&fs::read(&registry).unwrap()).unwrap();
     let installed_root = PathBuf::from(entries[0]["plugin_root"].as_str().unwrap());
@@ -681,7 +681,7 @@
         .unwrap()
         .file_name()
         .unwrap();
-    let legacy_root = config_home.join("herdr-dev/plugins/github").join(component);
+    let legacy_root = config_home.join(app_dir_name()).join("plugins/github").join(component);
     fs::create_dir_all(legacy_root.parent().unwrap()).unwrap();
     fs::rename(&installed_root, &legacy_root).unwrap();
     entries[0]["plugin_root"] = serde_json::json!(legacy_root);
@@ -798,7 +798,7 @@

     // Fail registry activation after the build, preserving the working version
     // and both running consumers' directories without a restore operation.
-    let registry = config_home.join("herdr-dev/plugins.json");
+    let registry = config_home.join(app_dir_name()).join("plugins.json");
     let original_registry = fs::read(&registry).unwrap();
     let block_write = format!("mkdir '{}'", registry.with_extension("json.tmp").display());
     fs::write(
@@ -995,8 +995,8 @@
         let stderr = String::from_utf8_lossy(&output.stderr);
         assert!(stderr.contains("usage: herdr plugin install"), "{stderr}");
         assert!(stderr.contains("[--ref REF] [--yes|-y]"), "{stderr}");
-        assert!(!config_home.join("herdr-dev/plugins").exists());
-        assert!(!config_home.join("herdr-dev/plugins.json").exists());
+        assert!(!config_home.join(app_dir_name()).join("plugins").exists());
+        assert!(!config_home.join(app_dir_name()).join("plugins.json").exists());
         assert!(!state_home.exists());
     }

@@ -1019,7 +1019,7 @@
 name = "Worktree Bootstrap"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]

 [[build]]
 command = ["sh", "-c", "echo built > built.txt; if [ -n \"$HERDR_SESSION\" ]; then echo \"$HERDR_SESSION\" > leaked-session.txt; fi"]
@@ -1155,7 +1155,7 @@
 name = "Build Fail"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]

 [[build]]
 command = ["sh", "-c", "echo before-fail && echo failed-build >&2 && exit 7"]
@@ -1251,7 +1251,7 @@
 name = "Missing Tool"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]

 [[build]]
 command = ["definitely-missing-herdr-build-tool-xyz"]
@@ -1340,7 +1340,7 @@
 name = "Manifest Mutator"
 version = "0.1.0"
 min_herdr_version = "0.6.10"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]

 [[build]]
 command = ["sh", "mutate.sh"]
@@ -1359,7 +1359,7 @@
 name = "Manifest Mutator"
 version = "0.1.0"
 min_herdr_version = "0.0.1"
-platforms = ["linux", "macos", "windows"]
+platforms = ["linux", "macos", "windows", "freebsd"]

 [[build]]
 command = ["sh", "mutate.sh"]
