--- /dev/null
+++ setuptools/tests/test_package_index_security.py
@@ -0,0 +1,127 @@
+"""Real child-process and bounded CPU controls for PackageIndex security."""
+import json
+import os
+from pathlib import Path
+import resource
+import subprocess
+import sys
+import tempfile
+import unittest
+import warnings
+
+from setuptools.package_index import PackageIndex, find_external_links
+
+
+class TestPackageIndexSecurity(unittest.TestCase):
+    def setUp(self):
+        self.temp = tempfile.TemporaryDirectory(prefix='setuptools-vcs-')
+        self.addCleanup(self.temp.cleanup)
+        self.root = Path(self.temp.name)
+        self.downloads = self.root / 'downloads with spaces'
+        self.downloads.mkdir()
+        commands = self.root / 'commands'
+        commands.mkdir()
+        self.record = self.root / 'argv.jsonl'
+        script = '''#!%s
+import json, os, sys
+with open(os.environ['SETUPTOOLS_TEST_ARGV'], 'a') as output:
+    output.write(json.dumps(sys.argv) + '\\n')
+raise SystemExit(int(os.environ.get('SETUPTOOLS_TEST_EXIT', '0')))
+''' % sys.executable
+        for name in ('git', 'hg', 'svn'):
+            command = commands / name
+            command.write_text(script)
+            command.chmod(0o755)
+        previous_env = os.environ.copy()
+        previous_cwd = os.getcwd()
+        os.environ['PATH'] = str(commands) + os.pathsep + os.environ['PATH']
+        os.environ['SETUPTOOLS_TEST_ARGV'] = str(self.record)
+        os.chdir(self.root)
+
+        def restore():
+            os.chdir(previous_cwd)
+            os.environ.clear()
+            os.environ.update(previous_env)
+
+        self.addCleanup(restore)
+        self.index = PackageIndex()
+
+    def records(self):
+        return [json.loads(line)[1:] for line in self.record.read_text().splitlines()]
+
+    def vcs_case(self, vcs, revision=False):
+        marker = 'marker-' + vcs + ('-revision' if revision else '-url')
+        injection = '$(touch${IFS}' + marker + ')'
+        remote = 'https://host.invalid/repo' + ('' if revision else injection)
+        ref = injection if revision else None
+        spec = vcs + '+' + remote + ('@' + ref if ref else '') + '#egg=fixture'
+        with warnings.catch_warnings():
+            warnings.simplefilter('ignore', UserWarning)
+            result = self.index.download(spec, self.downloads)
+        self.assertFalse((self.root / marker).exists(), 'shell substitution executed')
+        filename = str(self.downloads / ('repo' + ('@' + ref if ref else injection)))
+        self.assertEqual(result, filename)
+        if vcs == 'svn':
+            expected = [['checkout', '-q', spec.split('#', 1)[0], filename]]
+        else:
+            expected = [['clone', '--quiet', remote, filename]]
+            if ref:
+                expected += ([['-C', filename, 'checkout', '--quiet', ref]]
+                             if vcs == 'git' else
+                             [['--cwd', filename, 'up', '-C', '-r', ref, '-q']])
+        self.assertEqual(self.records(), expected)
+
+    def test_git_url_shell_characters_are_literal_arguments(self):
+        self.vcs_case('git')
+
+    def test_git_revision_shell_characters_are_literal_arguments(self):
+        self.vcs_case('git', revision=True)
+
+    def test_hg_url_shell_characters_are_literal_arguments(self):
+        self.vcs_case('hg')
+
+    def test_hg_revision_shell_characters_are_literal_arguments(self):
+        self.vcs_case('hg', revision=True)
+
+    def test_svn_url_shell_characters_are_literal_arguments(self):
+        self.vcs_case('svn')
+
+    def test_ordinary_clone_and_revision_keep_argument_order(self):
+        spec = 'git+https://host.invalid/repo@release-1#egg=fixture'
+        result = self.index.download(spec, self.downloads)
+        filename = str(self.downloads / 'repo@release-1')
+        self.assertEqual(result, filename)
+        self.assertEqual(self.records(), [
+            ['clone', '--quiet', 'https://host.invalid/repo', filename],
+            ['-C', filename, 'checkout', '--quiet', 'release-1'],
+        ])
+
+    def test_unsuccessful_vcs_command_is_reported(self):
+        os.environ['SETUPTOOLS_TEST_EXIT'] = '17'
+        with self.assertRaises(subprocess.CalledProcessError) as raised:
+            self.index.download('git+https://host.invalid/repo', self.downloads)
+        self.assertEqual(raised.exception.returncode, 17)
+
+    def test_ordinary_external_link_is_preserved(self):
+        html = '<a href="package.tar.gz" rel="download">package</a>'
+        self.assertEqual(list(find_external_links('https://host.invalid/', html)),
+                         ['https://host.invalid/package.tar.gz'])
+
+    def test_malformed_link_finishes_within_bounded_cpu_budget(self):
+        def limits():
+            resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
+
+        code = (
+            'import signal; from setuptools.package_index import find_external_links; '
+            'signal.setitimer(signal.ITIMER_PROF, 1); '
+            'assert list(find_external_links("https://host.invalid/", '
+            '"<a rel=" + " " * 150000)) == []'
+        )
+        result = subprocess.run([sys.executable, '-B', '-c', code],
+                                capture_output=True, text=True,
+                                preexec_fn=limits, timeout=30)
+        self.assertEqual(result.returncode, 0, (result.returncode, result.stderr))
+
+
+if __name__ == '__main__':
+    unittest.main()
