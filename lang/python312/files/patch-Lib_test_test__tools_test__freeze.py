# A reconfigured freeze installation must not write to the outer ports prefix.
--- Lib/test/test_tools/test_freeze.py.orig
+++ Lib/test/test_tools/test_freeze.py
@@ -1,5 +1,6 @@
 """Sanity-check tests for the "freeze" tool."""
 
+import os
 import sys
 import textwrap
 import unittest
@@ -30,8 +31,13 @@
             print('running...')
             sys.exit(0)
             """)
-        with os_helper.temp_dir() as outdir:
-            outdir, scriptfile, python = helper.prepare(script, outdir)
-            executable = helper.freeze(python, scriptfile, outdir)
-            text = helper.run(executable)
-        self.assertEqual(text, 'running...')
+        # A ports PREFIX inherited by make must not override configure --prefix.
+        with os_helper.temp_dir() as external_prefix:
+            with os_helper.EnvironmentVarGuard() as env:
+                env['PREFIX'] = external_prefix
+                with os_helper.temp_dir() as outdir:
+                    outdir, scriptfile, python = helper.prepare(script, outdir)
+                    executable = helper.freeze(python, scriptfile, outdir)
+                    text = helper.run(executable)
+                self.assertEqual(text, 'running...')
+            self.assertEqual(os.listdir(external_prefix), [])
