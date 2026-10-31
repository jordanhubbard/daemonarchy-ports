--- src/backend/drm/DRM.cpp.orig	2026-10-31 08:00:00.000000000 -0700
+++ src/backend/drm/DRM.cpp	2026-10-31 08:00:00.000000000 -0700
@@ -1,5 +1,6 @@
 #include "aquamarine/output/Output.hpp"
 #include <algorithm>
+#include <string_view>
 #include <aquamarine/backend/DRM.hpp>
 #include <aquamarine/backend/drm/Legacy.hpp>
 #include <aquamarine/backend/drm/Atomic.hpp>
@@ -95,14 +96,32 @@
     return enumerate;
 }
 
+// The kernel keeps connector status current from hotplug interrupts, so the cached status is
+// reliable for the drivers that probe every connector when they load (i915, amdgpu, ...).
+// nvidia-drm does not make that promise, so its connectors are always probed.
+static bool connectorStatusCached(int fd) {
+    auto ver = drmGetVersion(fd);
+    if (!ver)
+        return false;
+
+    const bool trusted = ver->name && std::string_view{ver->name} != "nvidia-drm";
+    drmFreeVersion(ver);
+    return trusted;
+}
+
 static int gpuNumBuiltinPanels(const SP<CSessionDevice> gpu) {
     auto resources = drmModeGetResources(gpu->fd);
     if (!resources)
         return 0;
 
-    int num = 0;
+    const bool cached = connectorStatusCached(gpu->fd);
+
+    int        num = 0;
     for (int i = 0; i < resources->count_connectors; ++i) {
-        auto drmConn = drmModeGetConnector(gpu->fd, resources->connectors[i]);
+        // Cached state is enough to count panels, and a forced probe makes the kernel
+        // run connector detection, which blocks for a long time on an unplugged DisplayPort
+        // output whose AUX channel does not answer (about 0.75 s on an i915 Type-C port).
+        auto drmConn = cached ? drmModeGetConnectorCurrent(gpu->fd, resources->connectors[i]) : drmModeGetConnector(gpu->fd, resources->connectors[i]);
         if (!drmConn)
             continue;
 
@@ -1136,7 +1155,7 @@
     listeners.gpuChange = gpu->events.change.listen([this](const CSessionDevice::SChangeEvent& E) {
         if (E.type == CSessionDevice::AQ_SESSION_EVENT_CHANGE_HOTPLUG) {
             backend->log(AQ_LOG_DEBUG, std::format("drm: Got a hotplug event for {}", gpuName));
-            recheckOutputs();
+            recheckOutputs(true);
         } else if (E.type == CSessionDevice::AQ_SESSION_EVENT_CHANGE_LEASE) {
             backend->log(AQ_LOG_DEBUG, std::format("drm: Got a lease event for {}", gpuName));
             scanLeases();
@@ -1202,8 +1221,8 @@
     }
 }
 
-void Aquamarine::CDRMBackend::recheckOutputs() {
-    scanConnectors();
+void Aquamarine::CDRMBackend::recheckOutputs(bool forceProbe) {
+    scanConnectors(forceProbe);
     markRedundantTiles();
 
     // disconnect now to possibly free up crtcs
@@ -1249,8 +1268,27 @@
         backend->log(AQ_LOG_ERROR, std::format("drm: Failed to update renderer state for {}", gpu->path));
 }
 
-void Aquamarine::CDRMBackend::scanConnectors() {
-    backend->log(AQ_LOG_DEBUG, std::format("drm: Scanning connectors for {}", gpu->path));
+// The kernel keeps connector state current from hotplug interrupts. Only a hotplug event
+// (or a cached state that cannot be used) needs a forced probe, which runs connector
+// detection and blocks for as long as an unplugged DisplayPort output takes to time out.
+static drmModeConnector* getConnector(int fd, uint32_t id, bool forceProbe, bool cached) {
+    if (forceProbe || !cached)
+        return drmModeGetConnector(fd, id);
+
+    auto current = drmModeGetConnectorCurrent(fd, id);
+    if (!current)
+        return nullptr;
+
+    if (current->connection == DRM_MODE_DISCONNECTED)
+        return current;
+
+    // Connected or unknown with nothing cached to go on: probe it.
+    drmModeFreeConnector(current);
+    return drmModeGetConnector(fd, id);
+}
+
+void Aquamarine::CDRMBackend::scanConnectors(bool forceProbe) {
+    backend->log(AQ_LOG_DEBUG, std::format("drm: Scanning connectors for {}{}", gpu->path, forceProbe ? " (probing)" : ""));
 
     auto resources = drmModeGetResources(gpu->fd);
     if (!resources) {
@@ -1258,11 +1296,13 @@
         return;
     }
 
+    const bool cached = connectorStatusCached(gpu->fd);
+
     for (int i = 0; i < resources->count_connectors; ++i) {
         uint32_t          connectorID = resources->connectors[i];
 
         SP<SDRMConnector> conn;
-        auto              drmConn = drmModeGetConnector(gpu->fd, connectorID);
+        auto              drmConn = getConnector(gpu->fd, connectorID, forceProbe, cached);
 
         backend->log(AQ_LOG_DEBUG, std::format("drm: Scanning connector id {}", connectorID));
 
