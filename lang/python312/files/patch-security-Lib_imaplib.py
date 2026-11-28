Backport CPython mail command validation (CVE-2025-15366 / CVE-2025-15367).
Upstream: GH-143922, GH-143924 and the GH-153067 IMAP compatibility correction.
Existing 3.12 test cases are retained; the new cases use local C0 enumeration.

--- Lib/imaplib.py.orig
+++ Lib/imaplib.py
@@ -132,3 +132,5 @@
 # We compile these in _mode_xxx.
 _Literal = br'.*{(?P<size>\d+)}$'
 _Untagged_status = br'\* (?P<data>\d+) (?P<type>[A-Z-]+)( (?P<data2>.*))?'
+# NUL, CR and LF cannot be represented even inside a quoted string.
+_control_chars = re.compile(b'[\x00\r\n]')
@@ -994,6 +996,8 @@
             if arg is None: continue
             if isinstance(arg, str):
                 arg = bytes(arg, self._encoding)
+            if _control_chars.search(arg):
+                raise ValueError("NUL, CR and LF not allowed in commands")
             data = data + b' ' + arg

         literal = self.literal
