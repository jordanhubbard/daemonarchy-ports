# Preserve the selected make implementation when rebuilding Python for freeze.
--- Tools/freeze/test/freeze.py.orig
+++ Tools/freeze/test/freeze.py
@@ -18,7 +18,8 @@
 TOOL_ROOT = os.path.dirname(TESTS_DIR)
 SRCDIR = get_python_source_dir()
 
-MAKE = shutil.which('make')
+# Rebuild with the selected make implementation, including nested builds.
+MAKE = shutil.which(os.environ.get('MAKE', 'make'))
 FREEZE = os.path.join(TOOL_ROOT, 'freeze.py')
 OUTDIR = os.path.join(TESTS_DIR, 'outdir')
 
