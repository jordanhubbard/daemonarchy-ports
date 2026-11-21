--- tests/support/mod.rs.orig
+++ tests/support/mod.rs
@@ -679,6 +679,118 @@
     }
 }

+#[cfg(target_os = "freebsd")]
+fn freebsd_processes(
+    option: &str,
+    category: &str,
+    pid: Option<u32>,
+) -> std::io::Result<serde_json::Map<String, serde_json::Value>> {
+    let mut command = std::process::Command::new("/usr/bin/procstat");
+    command.args(["--libxo", "json", option]);
+    match pid {
+        Some(pid) => {
+            command.arg(pid.to_string());
+        }
+        None => {
+            command.arg("-a");
+        }
+    }
+    let output = command.output()?;
+    if !output.status.success() {
+        return Err(std::io::Error::new(
+            std::io::ErrorKind::NotFound,
+            "procstat could not inspect the process",
+        ));
+    }
+    let mut value: serde_json::Value = serde_json::from_slice(&output.stdout)
+        .map_err(|err| std::io::Error::new(std::io::ErrorKind::InvalidData, err))?;
+    if value.get("__version").and_then(serde_json::Value::as_str) != Some("1") {
+        return Err(std::io::Error::new(
+            std::io::ErrorKind::InvalidData,
+            "unsupported procstat JSON version",
+        ));
+    }
+    value
+        .get_mut("procstat")
+        .and_then(|v| v.get_mut(category))
+        .and_then(serde_json::Value::as_object_mut)
+        .map(std::mem::take)
+        .ok_or_else(|| {
+            std::io::Error::new(
+                std::io::ErrorKind::InvalidData,
+                "missing procstat process table",
+            )
+        })
+}
+
+#[cfg(target_os = "freebsd")]
+fn freebsd_process_strings(pid: u32, option: &str, category: &str) -> std::io::Result<Vec<String>> {
+    let row = freebsd_processes(option, category, Some(pid))?
+        .remove(&pid.to_string())
+        .ok_or_else(|| {
+            std::io::Error::new(
+                std::io::ErrorKind::NotFound,
+                "process exited during inspection",
+            )
+        })?;
+    if row.get("process_id").and_then(serde_json::Value::as_u64) != Some(u64::from(pid)) {
+        return Err(std::io::Error::new(
+            std::io::ErrorKind::InvalidData,
+            "procstat process identity differs",
+        ));
+    }
+    row.get(category)
+        .and_then(serde_json::Value::as_array)
+        .ok_or_else(|| {
+            std::io::Error::new(
+                std::io::ErrorKind::InvalidData,
+                "missing procstat string vector",
+            )
+        })?
+        .iter()
+        .map(|v| {
+            v.as_str()
+                .filter(|s| !s.contains('\0'))
+                .map(str::to_owned)
+                .ok_or_else(|| {
+                    std::io::Error::new(std::io::ErrorKind::InvalidData, "invalid procstat string")
+                })
+        })
+        .collect()
+}
+
+#[cfg(target_os = "freebsd")]
+fn iter_worktree_server_pids() -> std::io::Result<Vec<u32>> {
+    let mut pids = Vec::new();
+    for (key, row) in freebsd_processes("-b", "binary", None)? {
+        let Some(pid) = row
+            .get("process_id")
+            .and_then(serde_json::Value::as_u64)
+            .and_then(|pid| u32::try_from(pid).ok())
+        else {
+            continue;
+        };
+        if pid == std::process::id() || key != pid.to_string() {
+            continue;
+        }
+        let Some(path) = row.get("pathname").and_then(serde_json::Value::as_str) else {
+            continue;
+        };
+        if is_test_herdr_binary(Path::new(path))
+            && read_cmdline(pid).is_ok_and(|args| args.iter().any(|arg| arg == "server"))
+        {
+            pids.push(pid);
+        }
+    }
+    Ok(pids)
+}
+
+#[cfg(target_os = "freebsd")]
+fn read_cmdline(pid: u32) -> std::io::Result<Vec<String>> {
+    freebsd_process_strings(pid, "-c", "arguments")
+}
+
+#[cfg(not(target_os = "freebsd"))]
 fn iter_worktree_server_pids() -> std::io::Result<Vec<u32>> {
     let own_pid = std::process::id();
     let mut pids = Vec::new();
@@ -708,6 +820,7 @@
     Ok(pids)
 }

+#[cfg(not(target_os = "freebsd"))]
 fn is_test_herdr_server_process(pid: u32) -> bool {
     let Some(exe_path) = proc_link_target(pid, "exe") else {
         return false;
@@ -724,10 +837,12 @@
     cmdline.iter().any(|arg| arg == "server")
 }

+#[cfg(not(target_os = "freebsd"))]
 fn proc_link_target(pid: u32, link: &str) -> Option<PathBuf> {
     fs::read_link(format!("/proc/{pid}/{link}")).ok()
 }

+#[cfg(not(target_os = "freebsd"))]
 fn read_cmdline(pid: u32) -> std::io::Result<Vec<String>> {
     let cmdline = fs::read(format!("/proc/{pid}/cmdline"))?;
     Ok(cmdline
@@ -738,6 +853,11 @@
 }

 fn process_runtime_dir(pid: u32) -> std::io::Result<Option<PathBuf>> {
+    #[cfg(target_os = "freebsd")]
+    let environ = freebsd_process_strings(pid, "-e", "environment")?
+        .join("\0")
+        .into_bytes();
+    #[cfg(not(target_os = "freebsd"))]
     let environ = fs::read(format!("/proc/{pid}/environ"))?;

     let mut socket_path: Option<PathBuf> = None;
