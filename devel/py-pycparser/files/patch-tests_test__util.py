--- tests/test_util.py.orig
+++ tests/test_util.py
@@ -15,7 +15,7 @@
 
 def cpp_supported():
     """Is cpp (the C preprocessor) supported as a native command?"""
-    return platform.system() == "Linux"
+    return platform.system() in ("Linux", "FreeBSD")
 
 
 def cpp_path():
