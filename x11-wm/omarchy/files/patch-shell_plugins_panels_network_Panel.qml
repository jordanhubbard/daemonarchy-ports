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
+    if (!debugStatus.grant) return ""
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
@@ -121,11 +160,11 @@
   // Keyboard focus zone for the panel. j/k crosses row boundaries:
   // header actions ⇄ band ⇄ DNS row ⇄ Wi-Fi networks. h/l move
   // within header actions, band pills, or DNS providers.
-  property string focusSection: "dns"  // "header" | "band" | "dns" | "wifi"
+  property string focusSection: hasNetworkManager ? "dns" : "header"  // "header" | "band" | "dns" | "wifi"
   property int headerIndex: 0
   readonly property bool canDisconnect: !!connectedWifiNetwork
   readonly property bool headerHasDisconnect: false
-  readonly property bool canShareWifi: info.type === "wifi" && canShareNetwork(connectedWifiNetwork)
+  readonly property bool canShareWifi: hasNetworkManager && info.type === "wifi" && canShareNetwork(connectedWifiNetwork)
   // The hero switch is the Wi-Fi radio, so it only exists when there is a
   // radio to switch. On a wired box it would otherwise sit there reading
   // "off" beside a perfectly live Ethernet connection.
@@ -137,7 +176,7 @@
   readonly property bool qrHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === qrHeaderIndex
   readonly property bool speedHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === speedHeaderIndex
   readonly property bool toggleHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === toggleHeaderIndex
-  readonly property string toggleHint: Networking.wifiEnabled ? "Turn Wi-Fi off" : "Turn Wi-Fi on"
+  readonly property string toggleHint: networking.wifiEnabled ? "Turn Wi-Fi off" : "Turn Wi-Fi on"
   readonly property var dnsProviders: ["DHCP", "Cloudflare", "Google", "Custom"]
   property int dnsIndex: 0
   // ["2.4", "5", ...], or empty when there is nothing to choose between.
@@ -149,7 +188,7 @@
   // Worth showing when there is a real choice, or when a pin is in force even
   // though only one band answers right now -- otherwise the Automatic switch
   // vanishes and the pin becomes unclearable from the panel.
-  readonly property bool canSelectBand: (kind === "wifi" || bandBusy)
+  readonly property bool canSelectBand: hasNetworkManager && (kind === "wifi" || bandBusy)
     && (bandAvailable.length > 1 || bandPinned)
   // While a change is in flight, show the state that was asked for rather than
   // the one still in force, so the row answers the click immediately instead of
@@ -163,7 +202,7 @@
   readonly property bool bandBusy: pendingBand !== ""
   // The speed test needs an interface to test, so its hero action only
   // appears once there is one.
-  readonly property bool canRunSpeedTest: !!info.iface
+  readonly property bool canRunSpeedTest: hasNetworkManager && !!info.iface
   property int bandIndex: 0
   // The band section has up to two cursor rows: the Automatic switch on the
   // header line, then the pills. Same shape as wifiActionFocused.
@@ -180,7 +219,7 @@
 
   onCanSelectBandChanged: {
     if (!canSelectBand && focusSection === "band") {
-      focusSection = "dns"
+      focusSection = root.hasNetworkManager ? "dns" : "header"
       bandAutoFocused = true
     }
   }
@@ -203,7 +242,7 @@
 
   function toggleNetwork() {
     if (!networkManagerAvailable) return
-    Networking.wifiEnabled = !Networking.wifiEnabled
+    networking.wifiEnabled = !networking.wifiEnabled
     Qt.callLater(function() { root.refresh(true) })
   }
 
@@ -322,7 +361,7 @@
       refresh(true)
       selectedIndex = wifiNetworks.length > 0 ? 0 : -1
       wifiActionFocused = false
-      focusSection = wifiNetworks.length > 0 ? "wifi" : "dns"
+      focusSection = wifiNetworks.length > 0 ? "wifi" : (hasNetworkManager ? "dns" : "header")
       var idx = dnsProviders.indexOf(dnsProvider)
       dnsIndex = idx >= 0 ? idx : 0
       syncBandIndex()
@@ -365,7 +404,7 @@
     if (wifiNetworks.length === 0) {
       selectedIndex = -1
       wifiActionFocused = false
-      if (focusSection === "wifi") focusSection = "dns"
+      if (focusSection === "wifi") focusSection = root.hasNetworkManager ? "dns" : "header"
     } else if (passwordSsid !== "") {
       var passwordIndex = wifiIndexForSsid(passwordSsid)
       if (passwordIndex >= 0) {
@@ -471,11 +510,11 @@
   function refresh(scanWifi) {
     if (scanWifi === undefined) scanWifi = false
     if (!detailsProc.running) detailsProc.running = true
-    if (!dnsProc.running) {
+    if (hasNetworkManager && !dnsProc.running) {
       dnsProc.command = ["bash", "-c", root.dnsCommand("")]
       dnsProc.running = true
     }
-    if (!bandProc.running) {
+    if (hasNetworkManager && !bandProc.running) {
       bandProc.command = ["omarchy-network-band"]
       bandProc.running = true
     }
@@ -638,7 +677,7 @@
   // reconnect is the thing you want to watch, and the details rows above
   // report it as it happens.
   function setBand(band) {
-    if (!band || actionProc.running) return
+    if (!hasNetworkManager || !band || actionProc.running) return
 
     root.pendingBand = band
     actionProc.command = ["omarchy-network-band", band]
@@ -665,7 +704,7 @@
   }
 
   function setDns(provider) {
-    if (!root.bar || !provider || actionProc.running) return
+    if (!hasNetworkManager || !root.bar || !provider || actionProc.running) return
 
     if (provider === "Custom") {
       var launcher = "omarchy-launch-floating-terminal-with-presentation"
@@ -681,6 +720,9 @@
   }
 
   function requiresCredentials(security) {
+    // The FreeBSD wizard collects CA/server verification and EAP-TLS material,
+    // rather than using NetworkManager's identity/password-only inline form.
+    if (!hasNetworkManager && (security === WifiSecurityType.WpaEap || security === WifiSecurityType.Wpa2Eap)) return false
     return Model.requiresCredentials(security, WifiSecurityType.Open, WifiSecurityType.Owe)
   }
 
@@ -757,7 +799,13 @@
   }
 
   function connectDirectly(ssid) {
-    runNetworkAction("connect", networkForSsid(ssid), function(network) { network.connect() })
+    var network = networkForSsid(ssid)
+    if (!hasNetworkManager && network && !network.known &&
+        (network.security === WifiSecurityType.WpaEap || network.security === WifiSecurityType.Wpa2Eap)) {
+      freebsdWifi.configureEnterprise(network)
+      return
+    }
+    runNetworkAction("connect", network, function(network) { network.connect() })
   }
 
   function connectWithPassphrase(ssid, passphrase) {
@@ -806,6 +854,23 @@
 
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
@@ -958,6 +1023,7 @@
     anchors.fill: parent
     bar: root.bar
     text: root.icon
+    tooltipText: root.debugSummary()
 
     onPressed: function(b) {
       if (root.opened) root.close()
@@ -969,6 +1035,17 @@
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
@@ -1009,7 +1086,7 @@
                 root.focusSection = "band"
                 root.bandAutoFocused = true
               } else {
-                root.focusSection = "dns"
+                root.focusSection = root.hasNetworkManager ? "dns" : "wifi"
               }
             }
           } else if (root.focusSection === "band") {
@@ -1025,7 +1102,7 @@
             } else if (root.bandAutoFocused && root.bandPillsVisible) {
               root.bandAutoFocused = false
             } else {
-              root.focusSection = "dns"
+              root.focusSection = root.hasNetworkManager ? "dns" : "header"
             }
           } else if (root.focusSection === "dns") {
             // k from DNS moves up into the band section when it's on screen,
@@ -1047,7 +1124,7 @@
             // k from the top row escapes back up to the DNS row rather than
             // wrapping around to the bottom of the list.
             if (dy < 0 && root.selectedIndex <= 0) {
-              root.focusSection = "dns"
+              root.focusSection = root.hasNetworkManager ? "dns" : "header"
               root.wifiActionFocused = false
             }
             else root.selectByDelta(dy)
@@ -1143,7 +1220,7 @@
           ToggleSwitch {
             id: powerSwitch
             visible: root.canToggleWifi
-            checked: Networking.wifiEnabled
+            checked: root.networking.wifiEnabled
             hasCursor: root.toggleHeaderHasCursor
             foreground: root.bar.foreground
             Layout.alignment: Qt.AlignVCenter
@@ -1267,6 +1344,53 @@
         }
       }
 
+      PanelSeparator {
+        visible: root.debugStatus.grant
+        foreground: root.bar.foreground
+      }
+
+      Column {
+        visible: root.debugStatus.grant
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
+          text: root.debugStatus.connected ? "Agent connected" :
+            (root.debugStatus.listener ? "Listening for approved agent" : "Approved; listener unavailable")
+          color: root.bar.foreground
+          font.family: root.bar.fontFamily
+          font.pixelSize: Style.font.bodySmall
+        }
+        Text {
+          width: parent.width
+          textFormat: Text.PlainText
+          text: root.debugAddress()
+          wrapMode: Text.WrapAnywhere
+          color: root.bar.foreground
+          font.family: root.bar.fontFamily
+          font.pixelSize: Style.font.bodySmall
+        }
+        Row {
+          spacing: Style.space(8)
+          Button {
+            visible: !root.debugStatus.listener
+            text: "Resume"
+            onClicked: root.bar.run("omarchy-launch-floating-terminal-with-presentation 'sudo daemonarchy-debug resume'")
+          }
+          Button {
+            text: "Unpair"
+            onClicked: root.bar.run("omarchy-launch-floating-terminal-with-presentation 'sudo daemonarchy-debug unpair'")
+          }
+        }
+      }
+
       // Wi-Fi band selection. Only on Wi-Fi, and only when the network answers
       // on more than one band -- a single-band AP has nothing to toggle.
       PanelSeparator {
@@ -1398,12 +1522,14 @@
 
       }
 
-      // DNS provider selection.
+      // NetworkManager DNS controls are unavailable on FreeBSD.
       PanelSeparator {
+        visible: root.hasNetworkManager
         foreground: root.bar.foreground
       }
 
       Column {
+        visible: root.hasNetworkManager
         width: parent.width
         spacing: Style.space(10)
 
@@ -1584,7 +1710,7 @@
     onHovered: function(isHovered) {
       if (!isHovered) return
       root.cursorActive = true
-      root.focusSection = "dns"
+      root.focusSection = root.hasNetworkManager ? "dns" : "header"
       root.dnsIndex = pill.index
     }
   }
