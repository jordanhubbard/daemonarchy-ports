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
