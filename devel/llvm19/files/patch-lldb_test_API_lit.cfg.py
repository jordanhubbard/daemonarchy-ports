--- lldb/test/API/lit.cfg.py.orig
+++ lldb/test/API/lit.cfg.py
@@ -327,6 +327,10 @@
 if "FREEBSD_LEGACY_PLUGIN" in os.environ:
     config.environment["FREEBSD_LEGACY_PLUGIN"] = os.environ["FREEBSD_LEGACY_PLUGIN"]
 
+# Preserve the configured shell for test accounts without a login shell.
+if "SHELL" in os.environ:
+    config.environment["SHELL"] = os.environ["SHELL"]
+
 # Propagate XDG_CACHE_HOME
 if "XDG_CACHE_HOME" in os.environ:
     config.environment["XDG_CACHE_HOME"] = os.environ["XDG_CACHE_HOME"]
