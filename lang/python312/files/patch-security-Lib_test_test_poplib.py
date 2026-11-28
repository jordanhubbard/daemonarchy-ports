Backport CPython mail command validation (CVE-2025-15366 / CVE-2025-15367).
Upstream: GH-143922, GH-143924 and the GH-153067 IMAP compatibility correction.
Existing 3.12 test cases are retained; the new cases use local C0 enumeration.

--- Lib/test/test_poplib.py.orig
+++ Lib/test/test_poplib.py
@@ -395,6 +395,16 @@
         self.assertIsNone(self.client.sock)
         self.assertIsNone(self.client.file)

+    def test_command_control_characters(self):
+        for codepoint in (*range(0x20), 0x7f):
+            char = chr(codepoint)
+            for command, value in ((self.client.user, f'user{char}'),
+                                   (self.client.pass_, f'{char}pass'),
+                                   (self.client.uidl, f'1{char}DELE 2')):
+                with self.subTest(codepoint=codepoint, command=command.__name__):
+                    with self.assertRaisesRegex(ValueError, 'Control characters'):
+                        command(value)
+
     @requires_ssl
     def test_stls_capa(self):
         capa = self.client.capa()
