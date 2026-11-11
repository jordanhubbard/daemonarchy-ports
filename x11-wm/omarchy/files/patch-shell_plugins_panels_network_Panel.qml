--- shell/plugins/panels/network/Panel.qml.orig
+++ shell/plugins/panels/network/Panel.qml
@@ -31,6 +31,52 @@

   // Live connection details from `ip` / /sys / iw.
   property var info: ({})  // { iface, type, ip, prefix, gateway, speed, duplex, ssid, signal, freq, bitrate, rx_bytes, tx_bytes, router_ping_ms, internet_ping_ms }
+  // The status file is refreshed every 10 s while daemonarchy-debug is alive. One
+  // that is missing, unparseable or older than this is not a state to act on.
+  readonly property int debugStatusMaxAge: 60
+  property string debugStatusText: ""
+  property var debugStatus: ({ unavailable: true, grant: false, listener: false, connected: false, addresses: "", port: "" })
+
+  function updateDebugStatus(content) {
+    var fields = {}
+    var lines = String(content || "").split("\n")
+    for (var i = 0; i < lines.length; i++) {
+      var separator = lines[i].indexOf("=")
+      if (separator > 0) fields[lines[i].slice(0, separator)] = lines[i].slice(separator + 1)
+    }
+    // Files from an older daemonarchy have no `updated=`: trust them as before.
+    var stale = false
+    if (fields.updated !== undefined) {
+      var updated = /^[0-9]{1,12}$/.test(fields.updated) ? Number(fields.updated) : NaN
+      stale = !(Math.abs(Date.now() / 1000 - updated) <= debugStatusMaxAge)
+    }
+    if (fields.schema !== "1" || stale) {
+      debugStatus = ({ unavailable: true, grant: false, listener: false, connected: false, addresses: "", port: "" })
+      return
+    }
+    debugStatus = ({
+      unavailable: false,
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
+    if (debugStatus.unavailable) return "Remote debugging: status unavailable"
+    if (!debugStatus.grant) return "Remote debugging: Off"
+    var state = debugStatus.connected ? "Agent connected" :
+      (debugStatus.listener ? "Listening for approved agent" : "Approved; listener unavailable")
+    return "Remote debugging: " + state + "\n" + debugAddress()
+  }

   // Throughput tracking. Rates are computed as deltas between successive
   // `omarchy-network-status --verbose` samples (~1.5s apart via detailsPoll).
@@ -66,8 +112,13 @@
     Commons.I18n.t("network.phrase7")
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
@@ -122,11 +173,11 @@
   // Keyboard focus zone for the panel. j/k crosses row boundaries:
   // header actions ⇄ portal ⇄ band ⇄ DNS row ⇄ Wi-Fi networks. h/l move
   // within header actions, band pills, or DNS providers.
-  property string focusSection: "dns"  // "header" | "portal" | "band" | "dns" | "wifi"
+  property string focusSection: hasNetworkManager ? "dns" : "header"  // "header" | "portal" | "band" | "dns" | "wifi"
   property int headerIndex: 0
   readonly property bool canDisconnect: !!connectedWifiNetwork
   readonly property bool headerHasDisconnect: false
-  readonly property bool canShareWifi: info.type === "wifi" && canShareNetwork(connectedWifiNetwork)
+  readonly property bool canShareWifi: hasNetworkManager && info.type === "wifi" && canShareNetwork(connectedWifiNetwork)
   // The hero switch is the Wi-Fi radio, so it only exists when there is a
   // radio to switch. On a wired box it would otherwise sit there reading
   // "off" beside a perfectly live Ethernet connection.
@@ -138,7 +189,7 @@
   readonly property bool qrHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === qrHeaderIndex
   readonly property bool speedHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === speedHeaderIndex
   readonly property bool toggleHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === toggleHeaderIndex
-  readonly property string toggleHint: Networking.wifiEnabled ? Commons.I18n.t("network.turnWifiOff") : Commons.I18n.t("network.turnWifiOn")
+  readonly property string toggleHint: networking.wifiEnabled ? Commons.I18n.t("network.turnWifiOff") : Commons.I18n.t("network.turnWifiOn")
   readonly property var dnsProviders: ["DHCP", "Cloudflare", "Google", "Custom"]
   property int dnsIndex: 0
   // ["2.4", "5", ...], or empty when there is nothing to choose between.
@@ -150,7 +201,7 @@
   // Worth showing when there is a real choice, or when a pin is in force even
   // though only one band answers right now -- otherwise the Automatic switch
   // vanishes and the pin becomes unclearable from the panel.
-  readonly property bool canSelectBand: (kind === "wifi" || bandBusy)
+  readonly property bool canSelectBand: hasNetworkManager && (kind === "wifi" || bandBusy)
     && (bandAvailable.length > 1 || bandPinned)
   // While a change is in flight, show the state that was asked for rather than
   // the one still in force, so the row answers the click immediately instead of
@@ -166,7 +217,7 @@
   readonly property string dnsEffective: pendingDnsProvider !== "" ? pendingDnsProvider : dnsProvider
   // The speed test needs an interface to test, so its hero action only
   // appears once there is one.
-  readonly property bool canRunSpeedTest: !!info.iface
+  readonly property bool canRunSpeedTest: hasNetworkManager && !!info.iface
   property int bandIndex: 0
   // The band section has up to two cursor rows: the Automatic switch on the
   // header line, then the pills. Same shape as wifiActionFocused.
@@ -183,7 +234,7 @@

   onCanSelectBandChanged: {
     if (!canSelectBand && focusSection === "band") {
-      focusSection = "dns"
+      focusSection = root.hasNetworkManager ? "dns" : "header"
       bandAutoFocused = true
     }
   }
@@ -206,7 +257,7 @@

   function toggleNetwork() {
     if (!networkManagerAvailable) return
-    Networking.wifiEnabled = !Networking.wifiEnabled
+    networking.wifiEnabled = !networking.wifiEnabled
     Qt.callLater(function() { root.refresh(true) })
   }

@@ -327,7 +378,7 @@
       refresh(true)
       selectedIndex = wifiNetworks.length > 0 ? 0 : -1
       wifiActionFocused = false
-      focusSection = hasCaptivePortal ? "portal" : (wifiNetworks.length > 0 ? "wifi" : "dns")
+      focusSection = hasCaptivePortal ? "portal" : (wifiNetworks.length > 0 ? "wifi" : (hasNetworkManager ? "dns" : "header"))
       var idx = dnsProviders.indexOf(dnsProvider)
       dnsIndex = idx >= 0 ? idx : 0
       syncBandIndex()
@@ -370,7 +421,7 @@
     if (wifiNetworks.length === 0) {
       selectedIndex = -1
       wifiActionFocused = false
-      if (focusSection === "wifi") focusSection = "dns"
+      if (focusSection === "wifi") focusSection = root.hasNetworkManager ? "dns" : "header"
     } else if (passwordSsid !== "") {
       var passwordIndex = wifiIndexForSsid(passwordSsid)
       if (passwordIndex >= 0) {
@@ -576,11 +627,11 @@
     checkConnectivity()
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
@@ -743,7 +794,7 @@
   // reconnect is the thing you want to watch, and the details rows above
   // report it as it happens.
   function setBand(band) {
-    if (!band || actionProc.running) return
+    if (!hasNetworkManager || !band || actionProc.running) return

     root.pendingBand = band
     actionProc.command = ["omarchy-network-band", band]
@@ -770,7 +821,7 @@
   }

   function setDns(provider) {
-    if (!root.bar || !provider || actionProc.running) return
+    if (!hasNetworkManager || !root.bar || !provider || actionProc.running) return

     if (provider === "Custom") {
       var launcher = "omarchy-launch-floating-terminal-with-presentation"
@@ -787,6 +838,9 @@
   }

   function requiresCredentials(security) {
+    // The FreeBSD wizard collects CA/server verification and EAP-TLS material,
+    // rather than using NetworkManager's identity/password-only inline form.
+    if (!hasNetworkManager && (security === WifiSecurityType.WpaEap || security === WifiSecurityType.Wpa2Eap)) return false
     return Model.requiresCredentials(security, WifiSecurityType.Open, WifiSecurityType.Owe)
   }

@@ -863,7 +917,13 @@
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
@@ -911,6 +971,27 @@
   implicitHeight: button.implicitHeight

   Component.onCompleted: refresh()
+
+  FileView {
+    id: debugStatusFile
+    path: "/var/run/daemonarchy-debug.status"
+    watchChanges: true
+    printErrors: false
+    onFileChanged: reload()
+    onLoaded: { root.debugStatusText = text(); root.updateDebugStatus(root.debugStatusText) }
+    onLoadFailed: { root.debugStatusText = ""; root.updateDebugStatus("") }
+  }
+
+  Timer {
+    interval: 5000
+    running: true
+    repeat: true
+    onTriggered: {
+      debugStatusFile.reload()
+      // Age the last text too, so a file that stops changing goes stale.
+      root.updateDebugStatus(root.debugStatusText)
+    }
+  }

   // Pulls everything we want about the active route's interface in one shot.
   Process {
@@ -1085,8 +1166,8 @@
     bar: root.bar
     text: root.icon
     active: root.restricted
-    tooltipText: root.hasCaptivePortal ? Commons.I18n.t("network.signInToNetwork")
-      : (root.restricted ? Commons.I18n.t("network.limitedAccess") : "")
+    tooltipText: [root.hasCaptivePortal ? Commons.I18n.t("network.signInToNetwork")
+      : (root.restricted ? Commons.I18n.t("network.limitedAccess") : ""), root.debugSummary()].filter(function(text) { return text !== "" }).join("\n")

     onPressed: function(b) {
       if (root.opened) root.close()
@@ -1096,6 +1177,17 @@
       // that deferral and stalling the open on NetworkManager's AP flood.
       else root.open()
     }
+  }
+
+  Rectangle {
+    visible: root.debugStatus.grant
+    anchors.right: button.right
+    anchors.top: button.top
+    width: Style.space(6)
+    height: width
+    radius: width / 2
+    color: root.debugStatus.connected ? Commons.Color.accent :
+      (root.debugStatus.listener ? root.bar.foreground : root.bar.urgent)
   }

   // Keyboard-driven popup anchored to the bar widget icon. The shared
@@ -1141,7 +1233,7 @@
                 root.focusSection = "band"
                 root.bandAutoFocused = true
               } else {
-                root.focusSection = "dns"
+                root.focusSection = root.hasNetworkManager ? "dns" : "wifi"
               }
             }
           } else if (root.focusSection === "portal") {
@@ -1167,7 +1259,7 @@
             } else if (root.bandAutoFocused && root.bandPillsVisible) {
               root.bandAutoFocused = false
             } else {
-              root.focusSection = "dns"
+              root.focusSection = root.hasNetworkManager ? "dns" : "header"
             }
           } else if (root.focusSection === "dns") {
             // k from DNS moves up into the band section when it's on screen,
@@ -1191,7 +1283,7 @@
             // k from the top row escapes back up to the DNS row rather than
             // wrapping around to the bottom of the list.
             if (dy < 0 && root.selectedIndex <= 0) {
-              root.focusSection = "dns"
+              root.focusSection = root.hasNetworkManager ? "dns" : "header"
               root.wifiActionFocused = false
             }
             else root.selectByDelta(dy)
@@ -1284,7 +1376,7 @@
           ToggleSwitch {
             id: powerSwitch
             visible: root.canToggleWifi
-            checked: Networking.wifiEnabled
+            checked: root.networking.wifiEnabled
             hasCursor: root.toggleHeaderHasCursor
             foreground: root.bar.foreground
             Layout.alignment: Qt.AlignVCenter
@@ -1435,6 +1527,60 @@
         }
       }

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
+          text: root.debugStatus.unavailable ? "Status unavailable" :
+            !root.debugStatus.grant ? "Off" :
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
+            visible: !root.debugStatus.unavailable && !root.debugStatus.grant
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
+
       // Wi-Fi band selection. Only on Wi-Fi, and only when the network answers
       // on more than one band -- a single-band AP has nothing to toggle.
       PanelSeparator {
@@ -1566,12 +1712,14 @@

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

@@ -1752,7 +1900,7 @@
     onHovered: function(isHovered) {
       if (!isHovered) return
       root.cursorActive = true
-      root.focusSection = "dns"
+      root.focusSection = root.hasNetworkManager ? "dns" : "header"
       root.dnsIndex = pill.index
     }
   }
