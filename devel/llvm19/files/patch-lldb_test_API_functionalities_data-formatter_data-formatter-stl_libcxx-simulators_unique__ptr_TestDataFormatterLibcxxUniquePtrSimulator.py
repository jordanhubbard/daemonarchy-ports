--- lldb/test/API/functionalities/data-formatter/data-formatter-stl/libcxx-simulators/unique_ptr/TestDataFormatterLibcxxUniquePtrSimulator.py.orig
+++ lldb/test/API/functionalities/data-formatter/data-formatter-stl/libcxx-simulators/unique_ptr/TestDataFormatterLibcxxUniquePtrSimulator.py
@@ -22,3 +22,26 @@
         self.expect(
             "frame variable var_with_deleter_up", substrs=["pointer =", "deleter ="]
         )
+
+    def _run_pair_revision(self, revision):
+        self.build(dictionary=dict(CXXFLAGS_EXTRAS="-DCOMPRESSED_PAIR_REV=%d" % revision))
+        lldbutil.run_to_source_breakpoint(
+            self, "Break here", lldb.SBFileSpec("main.cpp")
+        )
+        self.expect("frame variable var_up", substrs=["pointer ="])
+        self.expect("frame variable var_up", substrs=["deleter ="], matching=False)
+        self.expect(
+            "frame variable var_with_deleter_up", substrs=["pointer =", "deleter ="]
+        )
+
+    def test_compressed_pair_revision_1(self):
+        self._run_pair_revision(1)
+
+    def test_compressed_pair_revision_2(self):
+        self._run_pair_revision(2)
+
+    def test_compressed_pair_revision_3(self):
+        self._run_pair_revision(3)
+
+    def test_compressed_pair_revision_4(self):
+        self._run_pair_revision(4)
