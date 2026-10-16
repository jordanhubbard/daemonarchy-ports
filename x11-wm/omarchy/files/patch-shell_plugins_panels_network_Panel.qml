--- shell/plugins/panels/network/Panel.qml.orig
+++ shell/plugins/panels/network/Panel.qml
@@ -65,8 +65,13 @@
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
@@ -137,7 +142,7 @@
   readonly property bool qrHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === qrHeaderIndex
   readonly property bool speedHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === speedHeaderIndex
   readonly property bool toggleHeaderHasCursor: cursorActive && focusSection === "header" && headerIndex === toggleHeaderIndex
-  readonly property string toggleHint: Networking.wifiEnabled ? "Turn Wi-Fi off" : "Turn Wi-Fi on"
+  readonly property string toggleHint: networking.wifiEnabled ? "Turn Wi-Fi off" : "Turn Wi-Fi on"
   readonly property var dnsProviders: ["DHCP", "Cloudflare", "Google", "Custom"]
   property int dnsIndex: 0
   // ["2.4", "5", ...], or empty when there is nothing to choose between.
@@ -203,7 +208,7 @@
 
   function toggleNetwork() {
     if (!networkManagerAvailable) return
-    Networking.wifiEnabled = !Networking.wifiEnabled
+    networking.wifiEnabled = !networking.wifiEnabled
     Qt.callLater(function() { root.refresh(true) })
   }
 
@@ -681,6 +686,9 @@
   }
 
   function requiresCredentials(security) {
+    // The FreeBSD wizard collects CA/server verification and EAP-TLS material,
+    // rather than using NetworkManager's identity/password-only inline form.
+    if (!hasNetworkManager && (security === WifiSecurityType.WpaEap || security === WifiSecurityType.Wpa2Eap)) return false
     return Model.requiresCredentials(security, WifiSecurityType.Open, WifiSecurityType.Owe)
   }
 
@@ -757,7 +765,13 @@
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
@@ -1143,7 +1157,7 @@
           ToggleSwitch {
             id: powerSwitch
             visible: root.canToggleWifi
-            checked: Networking.wifiEnabled
+            checked: root.networking.wifiEnabled
             hasCursor: root.toggleHeaderHasCursor
             foreground: root.bar.foreground
             Layout.alignment: Qt.AlignVCenter
