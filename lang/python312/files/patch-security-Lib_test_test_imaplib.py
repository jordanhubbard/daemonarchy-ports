Backport CPython mail command validation (CVE-2025-15366 / CVE-2025-15367).
Upstream: GH-143922, GH-143924 and the GH-153067 IMAP compatibility correction.
Existing 3.12 test cases are retained; the new cases use local C0 enumeration.

--- Lib/test/test_imaplib.py.orig
+++ Lib/test/test_imaplib.py
@@ -538,6 +538,29 @@
         self.assertEqual(client.state, 'AUTH')


+    def test_command_control_characters(self):
+        client, _ = self._setup(SimpleIMAPHandler)
+        client.login('user', 'pass')
+        for char in '\0\r\n':
+            for mailbox in (f'a{char}b', f'a{char}b'.encode('ascii')):
+                with self.subTest(mailbox=mailbox):
+                    with self.assertRaisesRegex(ValueError, 'NUL, CR and LF'):
+                        client.select(mailbox)
+
+    def test_quoted_command_other_control_characters(self):
+        client, _ = self._setup(SimpleIMAPHandler)
+        client.login('user', 'pass')
+        # Preserve the upstream compatibility correction without adding the
+        # later automatic-quoting API changes to the 3.12 branch.
+        for codepoint in (*range(0x20), 0x7f):
+            char = chr(codepoint)
+            if char in '\0\r\n':
+                continue
+            with self.subTest(codepoint=codepoint):
+                typ, _ = client.select(f'"a{char}b"')
+                self.assertEqual(typ, 'OK')
+
+
 class NewIMAPTests(NewIMAPTestsMixin, unittest.TestCase):
     imap_class = imaplib.IMAP4
     server_class = socketserver.TCPServer
