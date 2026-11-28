--- lldb/test/API/functionalities/data-formatter/data-formatter-stl/libcxx-simulators/string/TestDataFormatterLibcxxStringSimulator.py.orig
+++ lldb/test/API/functionalities/data-formatter/data-formatter-stl/libcxx-simulators/string/TestDataFormatterLibcxxStringSimulator.py
@@ -24,13 +24,17 @@
 
 
 for v in [None, "ALTERNATE_LAYOUT"]:
-    for r in range(5):
-        name = "test_r%d" % r
-        defines = ["REVISION=%d" % r]
-        if v:
-            name += "_" + v
-            defines += [v]
-        f = functools.partialmethod(
-            LibcxxStringDataFormatterSimulatorTestCase._run_test, defines
-        )
-        setattr(LibcxxStringDataFormatterSimulatorTestCase, name, f)
+    for r in range(6):
+        for c in range(5):
+            # Keep the ten original method names and their original defines.
+            name = "test_r%d" % r if c == 0 else "test_r%d_c%d" % (r, c)
+            defines = ["REVISION=%d" % r]
+            if c:
+                defines += ["COMPRESSED_PAIR_REV=%d" % c]
+            if v:
+                name += "_" + v
+                defines += [v]
+            f = functools.partialmethod(
+                LibcxxStringDataFormatterSimulatorTestCase._run_test, defines
+            )
+            setattr(LibcxxStringDataFormatterSimulatorTestCase, name, f)
