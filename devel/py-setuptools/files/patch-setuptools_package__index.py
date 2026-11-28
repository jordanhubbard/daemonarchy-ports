# Backport CVE-2025-47273 (upstream 250a6d1), CVE-2022-40897 (43a9c9b),
# and argv subprocess execution from CVE-2024-6345 (88807c7).
# Retain the older SVN API, and compare resolved containment components.
--- setuptools/package_index.py.orig
+++ setuptools/package_index.py
@@ -1,4 +1,5 @@
 """PyPI and direct package downloading"""
 import sys
+import subprocess
 import os
 import re
@@ -198,5 +199,5 @@


-REL = re.compile(r"""<([^>]*\srel\s*=\s*['"]?([^'">]+)[^>]*)>""", re.I)
+REL = re.compile(r"""<([^>]*\srel\s{0,10}=\s{0,10}['"]?([^'" >]+)[^>]*)>""", re.I)
 # this line is here to fix emacs' cruddy broken syntax highlighting

@@ -800,4 +801,10 @@
         filename = os.path.join(tmpdir, name)

+        # CVE-2025-47273: decoding a URL may produce an absolute filename.
+        # Compare resolved path components, including any existing symlinks.
+        root = os.path.realpath(tmpdir)
+        if os.path.commonpath((root, os.path.realpath(filename))) != root:
+            raise ValueError(f"Invalid filename {filename}")
+
         # Download the file
         #
@@ -842,5 +849,5 @@
         warnings.warn("SVN download support is deprecated", UserWarning)
         url = url.split('#', 1)[0]  # remove any fragment for svn's sake
-        creds = ''
+        creds = []
         if url.lower().startswith('svn:') and '@' in url:
             scheme, netloc, path, p, q, f = urllib.parse.urlparse(url)
@@ -851,12 +858,12 @@
                     if ':' in auth:
                         user, pw = auth.split(':', 1)
-                        creds = " --username=%s --password=%s" % (user, pw)
+                        creds = ["--username=" + user, "--password=" + pw]
                     else:
-                        creds = " --username=" + auth
+                        creds = ["--username=" + auth]
                     netloc = host
                     parts = scheme, netloc, url, p, q, f
                     url = urllib.parse.urlunparse(parts)
         self.info("Doing subversion checkout from %s to %s", url, filename)
-        os.system("svn checkout%s -q %s %s" % (creds, url, filename))
+        subprocess.check_call(['svn', 'checkout', *creds, '-q', url, filename])
         return filename

@@ -884,12 +891,11 @@

         self.info("Doing git clone from %s to %s", url, filename)
-        os.system("git clone --quiet %s %s" % (url, filename))
+        subprocess.check_call(['git', 'clone', '--quiet', url, filename])

         if rev is not None:
             self.info("Checking out %s", rev)
-            os.system("git -C %s checkout --quiet %s" % (
-                filename,
-                rev,
-            ))
+            subprocess.check_call([
+                'git', '-C', filename, 'checkout', '--quiet', rev,
+            ])

         return filename
@@ -900,12 +906,11 @@

         self.info("Doing hg clone from %s to %s", url, filename)
-        os.system("hg clone --quiet %s %s" % (url, filename))
+        subprocess.check_call(['hg', 'clone', '--quiet', url, filename])

         if rev is not None:
             self.info("Updating to %s", rev)
-            os.system("hg --cwd %s up -C -r %s -q" % (
-                filename,
-                rev,
-            ))
+            subprocess.check_call([
+                'hg', '--cwd', filename, 'up', '-C', '-r', rev, '-q',
+            ])

         return filename
