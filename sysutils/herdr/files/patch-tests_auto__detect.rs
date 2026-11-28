--- tests/auto_detect.rs.orig
+++ tests/auto_detect.rs
@@ -750,3 +750,49 @@

     cleanup_spawned_herdr(server, base);
 }
+
+#[cfg(target_os = "freebsd")]
+#[test]
+fn cleanup_stops_only_owned_runtime() {
+    let _lock = test_lock();
+    let base = unique_test_dir();
+    let owned = base.join("owned scope");
+    let other = base.join("other scope");
+    let owned_runtime = owned.join("runtime");
+    let other_runtime = other.join("runtime");
+    let owned_socket = owned_runtime.join("herdr.sock");
+    let other_socket = other_runtime.join("herdr.sock");
+    let first = spawn_server(
+        &owned.join("config"),
+        &owned_runtime,
+        &owned_socket,
+        &owned_runtime.join("herdr-client.sock"),
+    );
+    let second = spawn_server(
+        &other.join("config"),
+        &other_runtime,
+        &other_socket,
+        &other_runtime.join("herdr-client.sock"),
+    );
+    let first_pid = first.child.process_id().unwrap();
+    let second_pid = second.child.process_id().unwrap();
+    wait_for_socket(&owned_socket, Duration::from_secs(5));
+    wait_for_socket(&other_socket, Duration::from_secs(5));
+
+    cleanup_test_base(&owned);
+    assert!(
+        wait_for_pid_exit(first_pid, Duration::from_secs(2)),
+        "cleanup must stop the owned runtime server"
+    );
+    assert!(
+        process_exists(second_pid),
+        "another runtime must remain alive"
+    );
+    assert!(ping_socket(&other_socket).contains("pong"));
+
+    cleanup_test_base(&other);
+    assert!(wait_for_pid_exit(second_pid, Duration::from_secs(2)));
+    drop(first);
+    drop(second);
+    let _ = fs::remove_dir_all(base);
+}
