--- /dev/null
+++ setuptools/tests/test_download_containment.py
@@ -0,0 +1,118 @@
+"""Exercise PackageIndex.download against real loopback HTTP responses."""
+import http.server
+import os
+from pathlib import Path
+import tempfile
+import threading
+import unittest
+import urllib.parse
+
+from setuptools.package_index import PackageIndex
+
+
+class TestDownloadContainment(unittest.TestCase):
+    def setUp(self):
+        self.temp = tempfile.TemporaryDirectory(prefix='setuptools-download-')
+        self.addCleanup(self.temp.cleanup)
+        self.fixture = Path(self.temp.name)
+        self.root = self.fixture / 'downloads'
+        self.root.mkdir()
+        self.requests = []
+        requests = self.requests
+
+        class Handler(http.server.BaseHTTPRequestHandler):
+            def do_GET(self):
+                requests.append(self.path)
+                self.send_response(200)
+                self.send_header('Content-Type', 'application/octet-stream')
+                self.end_headers()
+                self.wfile.write(b'private-package-fixture\n')
+
+            def log_message(self, *args):
+                pass
+
+        self.server = http.server.HTTPServer(('127.0.0.1', 0), Handler)
+        thread = threading.Thread(
+            target=lambda: self.server.serve_forever(poll_interval=0.01)
+        )
+        thread.start()
+
+        def close():
+            self.server.shutdown()
+            thread.join(5)
+            self.server.server_close()
+            self.assertFalse(thread.is_alive())
+
+        self.addCleanup(close)
+        self.base = 'http://127.0.0.1:%s/' % self.server.server_port
+        self.index = PackageIndex(index_url=self.base)
+
+    def download(self, name, root=None):
+        url = self.base + urllib.parse.quote(name, safe='')
+        return Path(self.index.download(url, self.root if root is None else root))
+
+    def assert_rejected(self, destination):
+        marker = b'owned-fixture-must-stay-unchanged\n'
+        destination.write_bytes(marker)
+        with self.assertRaisesRegex(ValueError, 'Invalid filename'):
+            self.download(str(destination))
+        self.assertEqual(destination.read_bytes(), marker)
+        self.assertEqual(self.requests, [])
+
+    def test_absolute_path_cannot_overwrite_outside_download_directory(self):
+        self.assert_rejected(self.fixture / 'outside.dat')
+
+    def test_directory_prefix_sibling_is_outside(self):
+        sibling = self.fixture / 'downloads-other'
+        sibling.mkdir()
+        self.assert_rejected(sibling / 'outside.dat')
+
+    def test_existing_symlink_cannot_redirect_outside(self):
+        destination = self.fixture / 'outside.dat'
+        marker = b'owned-fixture-must-stay-unchanged\n'
+        destination.write_bytes(marker)
+        (self.root / 'linked.dat').symlink_to(destination)
+        with self.assertRaisesRegex(ValueError, 'Invalid filename'):
+            self.download('linked.dat')
+        self.assertEqual(destination.read_bytes(), marker)
+        self.assertEqual(self.requests, [])
+
+    def test_ordinary_filename_downloads(self):
+        result = self.download('package-1.0.tar.gz')
+        self.assertEqual(result, self.root / 'package-1.0.tar.gz')
+        self.assertEqual(result.read_bytes(), b'private-package-fixture\n')
+        self.assertEqual(len(self.requests), 1)
+
+    def test_encoded_spaces_download(self):
+        result = self.download('package with spaces.tar.gz')
+        self.assertEqual(result, self.root / 'package with spaces.tar.gz')
+        self.assertEqual(result.read_bytes(), b'private-package-fixture\n')
+
+    def test_egg_zip_keeps_existing_suffix_normalization(self):
+        result = self.download('package-1.0.egg.zip')
+        self.assertEqual(result, self.root / 'package-1.0.egg')
+        self.assertEqual(result.read_bytes(), b'private-package-fixture\n')
+
+    def test_empty_filename_keeps_existing_default(self):
+        result = self.download('')
+        self.assertEqual(result, self.root / '__downloaded__')
+        self.assertEqual(result.read_bytes(), b'private-package-fixture\n')
+
+    def test_absolute_destination_inside_download_directory(self):
+        result = self.download(str(self.root / 'inside.tar.gz'))
+        self.assertEqual(result, self.root / 'inside.tar.gz')
+        self.assertEqual(result.read_bytes(), b'private-package-fixture\n')
+
+    def test_relative_download_directory(self):
+        previous = os.getcwd()
+        try:
+            os.chdir(self.fixture)
+            result = self.download('package.tar.gz', Path('downloads'))
+            self.assertEqual(result, Path('downloads/package.tar.gz'))
+            self.assertEqual(result.read_bytes(), b'private-package-fixture\n')
+        finally:
+            os.chdir(previous)
+
+
+if __name__ == '__main__':
+    unittest.main()
