# Backport the containment check from upstream 250a6d17978f9f6ac3ac887091f2d32886fbbb0b.
# Compare resolved components to reject prefix siblings and escaping symlinks.
--- setuptools/package_index.py.orig
+++ setuptools/package_index.py
@@ -799,6 +799,12 @@

         filename = os.path.join(tmpdir, name)

+        # CVE-2025-47273: decoding a URL may produce an absolute filename.
+        # Compare resolved path components, including any existing symlinks.
+        root = os.path.realpath(tmpdir)
+        if os.path.commonpath((root, os.path.realpath(filename))) != root:
+            raise ValueError(f"Invalid filename {filename}")
+
         # Download the file
         #
         if scheme == 'svn' or scheme.startswith('svn+'):
