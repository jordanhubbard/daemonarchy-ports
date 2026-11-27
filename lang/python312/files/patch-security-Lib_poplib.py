Backport CPython mail command validation (CVE-2025-15366 / CVE-2025-15367).
Upstream: GH-143922, GH-143924 and the GH-153067 IMAP compatibility correction.
Existing 3.12 test cases are retained; the new cases use local C0 enumeration.

--- Lib/poplib.py.orig
+++ Lib/poplib.py
@@ -122,4 +122,6 @@
     def _putcmd(self, line):
         if self._debugging: print('*cmd*', repr(line))
         line = bytes(line, self.encoding)
+        if re.search(b'[\x00-\x1F\x7F]', line):
+            raise ValueError('Control characters not allowed in commands')
         self._putline(line)
