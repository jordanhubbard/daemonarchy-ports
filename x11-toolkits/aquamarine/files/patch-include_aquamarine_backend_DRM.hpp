--- include/aquamarine/backend/DRM.hpp.orig	2026-10-31 08:00:00.000000000 -0700
+++ include/aquamarine/backend/DRM.hpp	2026-10-31 08:00:00.000000000 -0700
@@ -505,10 +505,10 @@
         bool     updateSecondaryRendererState(DRMFBList* retired = nullptr);
         bool     grabFormats();
         bool     shouldBlit();
-        void     scanConnectors();
+        void     scanConnectors(bool forceProbe = false);
         void     scanLeases();
         void     restoreAfterVT();
-        void     recheckOutputs();
+        void     recheckOutputs(bool forceProbe = false);
         void     recheckCRTCs();
         void     markRedundantTiles();
         void     buildGlFormats(const std::vector<SGLFormat>& fmts);
