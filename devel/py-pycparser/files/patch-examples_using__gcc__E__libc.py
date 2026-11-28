--- examples/using_gcc_E_libc.py.orig
+++ examples/using_gcc_E_libc.py
@@ -3,11 +3,12 @@
 #
 # Similar to the using_cpp_libc.py example, but uses 'gcc -E' instead
 # of 'cpp'. The same can be achieved with Clang instead of gcc. If you have
-# Clang installed, simply replace 'gcc' with 'clang' here.
+# Clang installed, set the CC environment variable to 'clang'.
 #
 # Eli Bendersky [https://eli.thegreenplace.net/]
 # License: BSD
 # -------------------------------------------------------------------------------
+import os
 import sys
 
 # This is not required if you've installed pycparser into
@@ -25,7 +26,7 @@
     ast = parse_file(
         filename,
         use_cpp=True,
-        cpp_path="gcc",
+        cpp_path=os.environ.get("CC", "gcc"),
         cpp_args=["-E", r"-Iutils/fake_libc_include"],
     )
     ast.show()
