--- src/backend/Backend.cpp.orig	2026-10-30 08:00:00.000000000 -0700
+++ src/backend/Backend.cpp	2026-10-30 08:00:00.000000000 -0700
@@ -116,6 +116,13 @@
     // Tear down implementations before the logger is destroyed,
     // as backends may log during teardown (e.g. SDRMConnector::disconnect).
     implementations.clear();
+
+    // The session owns libinput, which logs while it is torn down (libinput_unref
+    // -> libinputLog -> backendInUse->log). Members are destroyed in reverse order
+    // of declaration and `logger` is declared after `session`, so without this
+    // the session would log through an already destroyed logger when the process
+    // exits (SIGBUS in CLogger::log from the static backendInUse's destructor).
+    session.reset();
 }
 
 bool Aquamarine::CBackend::start() {
