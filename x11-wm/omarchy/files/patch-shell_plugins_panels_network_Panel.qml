--- shell/plugins/panels/network/Panel.qml.orig
+++ shell/plugins/panels/network/Panel.qml
@@ -30,6 +30,40 @@
 
   // Live connection details from `ip` / /sys / iw.
   property var info: ({})  // { iface, type, ip, prefix, gateway, speed, duplex, ssid, signal, freq, bitrate, rx_bytes, tx_bytes, router_ping_ms, internet_ping_ms }
+  property var debugStatus: ({ grant: false, listener: false, connected: false, addresses: "", port: "" })
+
+  function updateDebugStatus(content) {
+    var fields = {}
+    var lines = String(content || "").split("\n")
+    for (var i = 0; i < lines.length; i++) {
+      var separator = lines[i].indexOf("=")
+      if (separator > 0) fields[lines[i].slice(0, separator)] = lines[i].slice(separator + 1)
+    }
+    if (fields.schema !== "1") {
+      debugStatus = ({ grant: false, listener: false, connected: false, addresses: "", port: "" })
+      return
+    }
+    debugStatus = ({
+      grant: fields.grant === "1",
+      listener: fields.listener === "1",
+      connected: fields.connected === "1",
+      addresses: /^[0-9. ]*$/.test(fields.addresses || "") ? fields.addresses : "",
+      port: /^[0-9]{1,5}$/.test(fields.port || "") ? fields.port : ""
+    })
+  }
+
+  function debugAddress() {
+    if (!debugStatus.addresses || !debugStatus.port) return "Address pending"
+    return debugStatus.addresses.split(" ").filter(function(address) { return address !== "" })
+      .map(function(address) { return address + ":" + debugStatus.port }).join(", ")
+  }
+
+  function debugSummary() {
+    if (!debugStatus.grant) return "Remote debugging: Off"
+    var state = debugStatus.connected ? "Agent connected" :
+      (debugStatus.listener ? "Listening for approved agent" : "Approved; listener unavailable")
+    return "Remote debugging: " + state + "\n" + debugAddress()
+  }
 
   // Throughput tracking. Rates are computed as deltas between successive
   // `omarchy-network-status --verbose` samples (~1.5s apart via detailsPoll).
@@ -65,8 +99,13 @@
     "Bending light",
   ]
   readonly property string connectionPhrase: connectionPhrases[connectionPhraseIndex % connectionPhrases.length]
-  readonly property bool networkManagerAvailable: Networking.backend === NetworkBackendType.NetworkManager
-  readonly property var networkDevices: Networking.devices ? Networking.devices.values : []
+  // FreeBSD has no NetworkManager: FreeBSDWifi drives wpa_supplicant and
+  // stands in for Quickshell.Networking with the same shape.
+  readonly property bool hasNetworkManager: Networking.backend === NetworkBackendType.NetworkManager
+  readonly property var networking: hasNetworkManager ? Networking : freebsdWifi
+  FreeBSDWifi { id: freebsdWifi; active: !root.hasNetworkManager; fast: root.opened }
+  readonly property bool networkManagerAvailable: hasNetworkManager || freebsdWifi.available
+  readonly property var networkDevices: networking.devices ? networking.devices.values : []
   readonly property var wifiDevice: findDevice(DeviceType.Wifi)
   readonly property var wifiNetworkObjects: wifiDevice && wifiDevice.networks ? wifiDevice.networks.values : []
   readonly property var connectedWifiNetwork: findConnectedWifiNetwork()
@@ -137,7 +176,7 @@
   readonly property bool qrHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === qrHeaderIndex
   readonly property bool speedHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === speedHeaderIndex
   readonly property bool toggleHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === toggleHeaderIndex
-  readonly property string toggleHint: Networking.wifiEnabled ? "Turn Wi-Fi off" : "Turn Wi-Fi on"
+  readonly property string toggleHint: networking.wifiEnabled ? "Turn Wi-Fi off" : "Turn Wi-Fi on"
   readonly property var dnsProviders: ["DHCP", "Cloudflare", "Google", "Custom"]
   property int dnsIndex: 0
   // ["2.4", "5", ...], or empty when there is nothing to choose between.
@@ -203,7 +242,7 @@
 
   function toggleNetwork() {
     if (!networkManagerAvailable) return
-    Networking.wifiEnabled = !Networking.wifiEnabled
+    networking.wifiEnabled = !networking.wifiEnabled
     Qt.callLater(function() { root.refresh(true) })
   }
 
@@ -806,6 +845,23 @@
 
   Component.onCompleted: refresh()
 
+  FileView {
+    id: debugStatusFile
+    path: "/var/run/daemonarchy-debug.status"
+    watchChanges: true
+    printErrors: false
+    onFileChanged: reload()
+    onLoaded: root.updateDebugStatus(text())
+    onLoadFailed: root.updateDebugStatus("")
+  }
+
+  Timer {
+    interval: 5000
+    running: true
+    repeat: true
+    onTriggered: debugStatusFile.reload()
+  }
+
   // Pulls everything we want about the active route's interface in one shot.
   Process {
     id: detailsProc
@@ -958,6 +1014,7 @@
     anchors.fill: parent
     bar: root.bar
     text: root.icon
+    tooltipText: root.debugSummary()
 
     onPressed: function(b) {
       if (root.opened) root.close()
@@ -969,6 +1026,17 @@
     }
   }
 
+  Rectangle {
+    visible: root.debugStatus.grant
+    anchors.right: button.right
+    anchors.top: button.top
+    width: Style.space(6)
+    height: width
+    radius: width / 2
+    color: root.debugStatus.connected ? Color.accent :
+      (root.debugStatus.listener ? root.bar.foreground : root.bar.urgent)
+  }
+
   // Keyboard-driven popup anchored to the bar widget icon. The shared
   // KeyboardPanel handles the layer-shell PanelWindow scaffolding
   // (focus priming on open, screen binding, anchored-to-icon positioning,
@@ -1143,7 +1211,7 @@
           ToggleSwitch {
             id: powerSwitch
             visible: root.canToggleWifi
-            checked: Networking.wifiEnabled
+            checked: root.networking.wifiEnabled
             hasCursor: root.toggleHeaderHasCursor
             foreground: root.bar.foreground
             Layout.alignment: Qt.AlignVCenter
@@ -1266,6 +1334,59 @@
           }
         }
       }
+
+      PanelSeparator {
+        foreground: root.bar.foreground
+      }
+
+      Column {
+        width: parent.width
+        spacing: Style.spacing.labelGap
+
+        PanelSectionHeader {
+          text: "REMOTE DEBUGGING"
+          foreground: root.bar.foreground
+          fontFamily: root.bar.fontFamily
+        }
+        Text {
+          width: parent.width
+          textFormat: Text.PlainText
+          text: !root.debugStatus.grant ? "Off" :
+            (root.debugStatus.connected ? "Agent connected" :
+            (root.debugStatus.listener ? "Listening for approved agent" : "Approved; listener unavailable"))
+          color: root.bar.foreground
+          font.family: root.bar.fontFamily
+          font.pixelSize: Style.font.bodySmall
+        }
+        Text {
+          width: parent.width
+          textFormat: Text.PlainText
+          text: root.debugAddress()
+          visible: root.debugStatus.grant
+          wrapMode: Text.WrapAnywhere
+          color: root.bar.foreground
+          font.family: root.bar.fontFamily
+          font.pixelSize: Style.font.bodySmall
+        }
+        Row {
+          spacing: Style.space(8)
+          Button {
+            visible: !root.debugStatus.grant
+            text: "Pair"
+            onClicked: root.bar.run("omarchy-launch-floating-terminal-with-presentation 'sudo daemonarchy-debug listen'")
+          }
+          Button {
+            visible: root.debugStatus.grant && !root.debugStatus.listener
+            text: "Resume"
+            onClicked: root.bar.run("omarchy-launch-floating-terminal-with-presentation 'sudo daemonarchy-debug resume'")
+          }
+          Button {
+            visible: root.debugStatus.grant
+            text: "Unpair"
+            onClicked: root.bar.run("omarchy-launch-floating-terminal-with-presentation 'sudo daemonarchy-debug unpair'")
+          }
+        }
+      }
 
       // Wi-Fi band selection. Only on Wi-Fi, and only when the network answers
       // on more than one band -- a single-band AP has nothing to toggle.
