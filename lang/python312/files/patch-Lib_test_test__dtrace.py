# Restore runtime probe assertions and current call coverage.
# FreeBSD uses native DOF inventory; keep every original trace expectation.
# Report the exact missing kernel process-provider refusal as unavailable.
--- Lib/test/test_dtrace.py.orig
+++ Lib/test/test_dtrace.py
@@ -4,6 +4,7 @@
 import subprocess
 import sys
 import sysconfig
+import tempfile
 import types
 import unittest
 
@@ -97,6 +98,21 @@
     EXTENSION = ".d"
     COMMAND = ["dtrace", "-q", "-s"]
 
+    @staticmethod
+    def raise_for_unavailable_process_provider(output):
+        # BEGIN can work even when the kernel has no process provider.
+        # This exact backend refusal is not a missing Python probe: all
+        # other tracer errors and original trace assertions remain failures.
+        if "pid provider is not installed on this system" in output:
+            raise unittest.SkipTest(
+                "DTrace kernel process provider is unavailable (fasttrap)")
+
+    def trace(self, script_file, subcommand=None):
+        output = super().trace(script_file, subcommand)
+        self.raise_for_unavailable_process_provider(output)
+        return output
+
+
 
 class SystemTapBackend(TraceBackend):
     EXTENSION = ".stp"
@@ -126,7 +142,11 @@
     def test_verify_call_opcodes(self):
         """Ensure our call stack test hits all function call opcodes"""
 
-        opcodes = set(["CALL_FUNCTION", "CALL_FUNCTION_EX", "CALL_FUNCTION_KW"])
+        # CALL handles positional and keyword calls in Python 3.12;
+        # KW_NAMES distinguishes the keyword form.
+        opcodes = {"CALL", "CALL_FUNCTION_EX", "KW_NAMES"}
+        positional_call = False
+        keyword_names = False
 
         with open(abspath("call_stack.py")) as f:
             code_string = f.read()
@@ -145,8 +165,82 @@
 
         for instruction in get_function_instructions('start'):
             opcodes.discard(instruction.opname)
+            if instruction.opname == "KW_NAMES":
+                keyword_names = True
+            elif instruction.opname == "CALL":
+                positional_call |= not keyword_names
+                keyword_names = False
 
         self.assertEqual(set(), opcodes)
+        self.assertTrue(positional_call,
+                        "call stack fixture must exercise positional CALL")
+
+    def test_preserve_exception_state(self):
+        """Probe metadata must not change Python exception semantics."""
+        program = r'''
+import sys
+import types
+
+namespace = {}
+exec(compile("def normal():\n return 17\ndef raising():\n raise KeyError(23)\n",
+             "virtual-\udc80.py", "exec"), namespace)
+safe = {}
+exec(compile("def normal():\n return 17\n", "virtual.py", "exec"), safe)
+renamed = types.FunctionType(
+    safe["normal"].__code__.replace(co_name="function-\udc80"), {})
+
+def check_return(function, label):
+    try:
+        raise ZeroDivisionError("preserve context")
+    except ZeroDivisionError as context:
+        if function() != 17:
+            raise AssertionError("return value changed")
+        if sys.exception() is not context:
+            raise AssertionError("handled exception context changed")
+    print("0\t" + label)
+
+check_return(namespace["normal"], "filename-return:ok")
+try:
+    raise ZeroDivisionError("preserve context")
+except ZeroDivisionError as context:
+    try:
+        namespace["raising"]()
+    except KeyError as error:
+        if error.args != (23,):
+            raise AssertionError("raised exception changed")
+    else:
+        raise AssertionError("KeyError was lost")
+    if sys.exception() is not context:
+        raise AssertionError("unwind exception context changed")
+print("0\tfilename-unwind:ok")
+check_return(renamed, "function-name:ok")
+'''
+        if self.backend.EXTENSION == ".d":
+            probes = '''
+python$target:::function-entry,
+python$target:::function-return,
+python$target:::line
+{ self->calls++; }
+'''
+        else:
+            probes = '''
+global calls
+probe process.mark("function__entry"),
+      process.mark("function__return"), process.mark("line")
+{ calls++; }
+'''
+        with tempfile.TemporaryDirectory() as directory:
+            python_file = os.path.join(directory, "metadata.py")
+            script_file = os.path.join(directory, "metadata" + self.backend.EXTENSION)
+            with open(python_file, "w") as stream:
+                stream.write(program)
+            with open(script_file, "w") as stream:
+                stream.write(probes)
+            output = self.backend.trace_python(
+                script_file, python_file, optimize_python=self.optimize_python)
+        self.assertEqual(
+            normalize_trace_output(output),
+            "filename-return:ok\nfilename-unwind:ok\nfunction-name:ok")
 
     def test_gc(self):
         self.run_case("gc")
@@ -178,6 +272,9 @@
     @classmethod
     def setUpClass(cls):
         if sysconfig.get_config_var('WITH_DTRACE'):
+            if sys.platform.startswith('freebsd'):
+                DTraceBackend().assert_usable()
+                return
             readelf_major_version, readelf_minor_version = cls.get_readelf_version()
             if support.verbose:
                 print(f"readelf version: {readelf_major_version}.{readelf_minor_version}")
@@ -215,7 +312,37 @@
 
         return int(match.group(1)), int(match.group(2))
 
+    def get_freebsd_probe_inventory(self):
+        # FreeBSD stores DOF rather than ELF SystemTap notes.  Query the
+        # providers of a fresh instance of this interpreter after DTrace
+        # has loaded its helper, including probes in a shared libpython.
+        target = subprocess.Popen(
+            [sys.executable, "-I", "-u", "-c",
+             "import sys; print('ready', flush=True); sys.stdin.readline()"],
+            stdin=subprocess.PIPE, stdout=subprocess.PIPE,
+            stderr=subprocess.PIPE, text=True)
+        try:
+            self.assertEqual(target.stdout.readline().strip(), "ready")
+            provider = "python" + str(target.pid)
+            result = subprocess.run(
+                ["dtrace", "-l", "-P", provider],
+                capture_output=True, text=True, timeout=support.SHORT_TIMEOUT)
+            DTraceBackend.raise_for_unavailable_process_provider(
+                result.stdout + result.stderr)
+            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
+            names = set()
+            for line in result.stdout.splitlines():
+                fields = line.split()
+                if len(fields) >= 5 and fields[0].isdigit() and fields[1] == provider:
+                    names.add(fields[-1].replace("-", "__"))
+            return "\n".join("Name: " + name for name in sorted(names))
+        finally:
+            _, stderr = target.communicate(input="\n", timeout=support.SHORT_TIMEOUT)
+            self.assertEqual(target.returncode, 0, stderr)
+
     def get_readelf_output(self):
+        if sys.platform.startswith('freebsd'):
+            return self.get_freebsd_probe_inventory()
         command = ["readelf", "-n", sys.executable]
         stdout, _ = subprocess.Popen(
             command,
@@ -240,7 +367,6 @@
             with self.subTest(probe_name=probe_name):
                 self.assertIn(probe_name, readelf_output)
 
-    @unittest.expectedFailure
     def test_missing_probes(self):
         readelf_output = self.get_readelf_output()
 
