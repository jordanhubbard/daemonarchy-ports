--- tap/parser.py.orig
+++ tap/parser.py
@@ -28,11 +28,11 @@
         \s*                    # Optional whitespace.
         (?P<directive>.*)      # Optional directive text.
     """
-    ok = re.compile(r"^ok" + result_base, re.VERBOSE)
-    not_ok = re.compile(r"^not\ ok" + result_base, re.VERBOSE)
+    ok = re.compile(r"^ok\b" + result_base, re.VERBOSE)
+    not_ok = re.compile(r"^not\ ok\b" + result_base, re.VERBOSE)
     plan = re.compile(
         r"""
-        ^1..(?P<expected>\d+) # Match the plan details.
+        ^1\.\.(?P<expected>\d+) # Match the plan details.
         [^#]*                 # Consume any non-hash character to confirm only
                               # directives appear with the plan details.
         \#?                   # Optional directive marker.
