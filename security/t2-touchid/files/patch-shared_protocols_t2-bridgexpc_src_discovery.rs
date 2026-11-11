--- shared/protocols/t2-bridgexpc/src/discovery.rs.orig	2026-10-28 20:04:07 UTC
+++ shared/protocols/t2-bridgexpc/src/discovery.rs
@@ -1,7 +1,9 @@
 // SPDX-License-Identifier: GPL-3.0-or-later
 
+#[cfg(target_os = "linux")]
 use std::fs;
 use std::net::Ipv6Addr;
+#[cfg(target_os = "linux")]
 use std::path::Path;
 use std::process::{Command, Stdio};
 
@@ -11,6 +13,7 @@ pub const LAST_DYNAMIC_PORT: u16 = 65535;
 pub const FIRST_DYNAMIC_PORT: u16 = 49152;
 pub const LAST_DYNAMIC_PORT: u16 = 65535;
 
+#[cfg(target_os = "linux")]
 pub fn interface(explicit: Option<String>) -> Result<String> {
     if let Some(interface) = explicit {
         return Ok(interface);
@@ -38,6 +41,60 @@ pub fn interface(explicit: Option<String>) -> Result<S
     }
 }
 
+/// FreeBSD: the T2 link is a ue(4) interface whose parent is cdce(4) attached
+/// to USB 05ac:8233 (net.ue.N.%parent, dev.cdce.M.%pnpinfo).
+#[cfg(target_os = "freebsd")]
+pub fn interface(explicit: Option<String>) -> Result<String> {
+    if let Some(interface) = explicit {
+        return Ok(interface);
+    }
+    let list = command_output("/sbin/ifconfig", &["-l"]).context("list network interfaces")?;
+    let matches: Vec<String> = list
+        .split_whitespace()
+        .filter(|name| apple_ncm_freebsd(name))
+        .map(str::to_owned)
+        .collect();
+    match matches.as_slice() {
+        [name] => Ok(name.clone()),
+        [] => bail!("no Apple 05ac:8233 CDC-NCM interface found"),
+        _ => bail!("multiple Apple CDC-NCM interfaces found; use --interface"),
+    }
+}
+
+#[cfg(target_os = "freebsd")]
+fn apple_ncm_freebsd(name: &str) -> bool {
+    let Some(unit) = name.strip_prefix("ue") else {
+        return false;
+    };
+    if unit.is_empty() || !unit.bytes().all(|b| b.is_ascii_digit()) {
+        return false;
+    }
+    let Ok(parent) = command_output("/sbin/sysctl", &["-n", &format!("net.ue.{unit}.%parent")]) else {
+        return false;
+    };
+    let Some(cdce) = parent.trim().strip_prefix("cdce") else {
+        return false;
+    };
+    let Ok(pnp) = command_output("/sbin/sysctl", &["-n", &format!("dev.cdce.{cdce}.%pnpinfo")]) else {
+        return false;
+    };
+    pnpinfo_is_t2(&pnp)
+}
+
+#[cfg_attr(not(target_os = "freebsd"), allow(dead_code))]
+fn pnpinfo_is_t2(pnp: &str) -> bool {
+    let fields: Vec<&str> = pnp.split_whitespace().collect();
+    fields.contains(&"vendor=0x05ac") && fields.contains(&"product=0x8233")
+}
+
+#[cfg(target_os = "freebsd")]
+fn command_output(program: &str, args: &[&str]) -> Result<String> {
+    let output = Command::new(program).args(args).stderr(Stdio::null()).output()?;
+    ensure!(output.status.success(), "{program} failed");
+    Ok(String::from_utf8_lossy(&output.stdout).into_owned())
+}
+
+#[cfg(target_os = "linux")]
 fn apple_ncm(path: &Path) -> bool {
     path.ancestors().any(|parent| {
         let vendor = fs::read_to_string(parent.join("idVendor")).ok();
@@ -51,6 +108,7 @@ fn apple_ncm(path: &Path) -> bool {
     })
 }
 
+#[cfg(target_os = "linux")]
 pub fn host(interface: &str, explicit: Option<String>) -> Result<Ipv6Addr> {
     ensure_link_local(interface)?;
     if let Some(host) = explicit {
@@ -90,6 +148,92 @@ pub fn host(interface: &str, explicit: Option<String>)
     DEFAULT_HOST.parse().context("invalid built-in T2 address")
 }
 
+/// FreeBSD: wake the neighbour cache with an all-nodes ping, then read it
+/// with ndp(8). The interface's own address is listed as "permanent".
+#[cfg(target_os = "freebsd")]
+pub fn host(interface: &str, explicit: Option<String>) -> Result<Ipv6Addr> {
+    ensure_link_local(interface)?;
+    if let Some(host) = explicit {
+        return host.parse().context("invalid IPv6 host");
+    }
+    let _ = Command::new("/sbin/ping")
+        .args(["-6", "-c", "1", "-t", "1", &format!("ff02::1%{interface}")])
+        .stdout(Stdio::null())
+        .stderr(Stdio::null())
+        .status();
+    if let Ok(table) = command_output("/usr/sbin/ndp", &["-an"]) {
+        if let [host] = ndp_neighbors(&table, interface).as_slice() {
+            return Ok(*host);
+        }
+    }
+    DEFAULT_HOST.parse().context("invalid built-in T2 address")
+}
+
+/// Link-local neighbours on `interface` from `ndp -an`:
+/// "Neighbor Linklayer-Address Netif Expire S Flags".
+#[cfg_attr(not(target_os = "freebsd"), allow(dead_code))]
+fn ndp_neighbors(table: &str, interface: &str) -> Vec<Ipv6Addr> {
+    let mut hosts: Vec<Ipv6Addr> = table
+        .lines()
+        .skip(1)
+        .filter_map(|line| {
+            let fields: Vec<&str> = line.split_whitespace().collect();
+            if fields.len() < 4
+                || fields[2] != interface
+                || fields[1] == "(incomplete)"
+                || fields[3] == "permanent"
+            {
+                return None;
+            }
+            fields[0].split('%').next()?.parse().ok()
+        })
+        .filter(|address: &Ipv6Addr| (address.segments()[0] & 0xffc0) == 0xfe80)
+        .collect();
+    hosts.sort_unstable();
+    hosts.dedup();
+    hosts
+}
+
+#[cfg(target_os = "freebsd")]
+fn ensure_link_local(interface: &str) -> Result<()> {
+    let addresses = command_output("/sbin/ifconfig", &[interface, "inet6"])
+        .context("IPv6 is unavailable; cannot reach the T2 link-local service")?;
+    let found = addresses.lines().any(|line| {
+        let mut fields = line.split_whitespace();
+        fields.next() == Some("inet6") && fields.next().is_some_and(|a| a.starts_with("fe80:"))
+    });
+    ensure!(
+        found,
+        "interface {interface} has no IPv6 link-local address; enable IPv6 on the CDC-NCM link \
+         (ifconfig {interface} inet6 -ifdisabled auto_linklocal)"
+    );
+    Ok(())
+}
+
+#[cfg(test)]
+mod tests {
+    use super::{ndp_neighbors, pnpinfo_is_t2};
+
+    #[test]
+    fn finds_the_t2_in_the_ndp_table() {
+        let table = "Neighbor                             Linklayer Address  Netif Expire    S Flags\n\
+                     fe80::aede:48ff:fe00:1122%ue0        ac:de:48:00:11:22    ue0 permanent R \n\
+                     fe80::aede:48ff:fe33:4455%ue0        ac:de:48:33:44:55    ue0 23h59m5s  S \n\
+                     fe80::1%ue1                          00:11:22:33:44:55    ue1 23h59m5s  S \n";
+        assert_eq!(
+            ndp_neighbors(table, "ue0"),
+            vec!["fe80::aede:48ff:fe33:4455".parse::<std::net::Ipv6Addr>().unwrap()]
+        );
+    }
+
+    #[test]
+    fn recognises_the_t2_usb_ids() {
+        assert!(pnpinfo_is_t2("vendor=0x05ac product=0x8233 devclass=0x00 intclass=0x02"));
+        assert!(!pnpinfo_is_t2("vendor=0x05ac product=0x12a8 devclass=0x00 intclass=0x02"));
+    }
+}
+
+#[cfg(target_os = "linux")]
 fn ensure_link_local(interface: &str) -> Result<()> {
     let addresses = fs::read_to_string("/proc/net/if_inet6")
         .context("IPv6 is unavailable; cannot reach the T2 link-local service")?;
