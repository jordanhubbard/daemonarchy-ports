--- setuptools/tests/test_packageindex.py.orig
+++ setuptools/tests/test_packageindex.py
@@ -197,56 +197,44 @@
         url = 'git+https://github.example/group/project@master#egg=foo'
         index = setuptools.package_index.PackageIndex()

-        with mock.patch("os.system") as os_system_mock:
+        with mock.patch("subprocess.check_call") as check_call:
             result = index.download(url, str(tmpdir))

-        os_system_mock.assert_called()
-
         expected_dir = str(tmpdir / 'project@master')
-        expected = (
-            'git clone --quiet '
-            'https://github.example/group/project {expected_dir}'
-        ).format(**locals())
-        first_call_args = os_system_mock.call_args_list[0][0]
-        assert first_call_args == (expected,)
-
-        tmpl = 'git -C {expected_dir} checkout --quiet master'
-        expected = tmpl.format(**locals())
-        assert os_system_mock.call_args_list[1][0] == (expected,)
+        assert check_call.call_args_list == [
+            mock.call(['git', 'clone', '--quiet',
+                       'https://github.example/group/project', expected_dir]),
+            mock.call(['git', '-C', expected_dir, 'checkout', '--quiet', 'master']),
+        ]
         assert result == expected_dir

     def test_download_git_no_rev(self, tmpdir):
         url = 'git+https://github.example/group/project#egg=foo'
         index = setuptools.package_index.PackageIndex()

-        with mock.patch("os.system") as os_system_mock:
+        with mock.patch("subprocess.check_call") as check_call:
             result = index.download(url, str(tmpdir))

-        os_system_mock.assert_called()
-
         expected_dir = str(tmpdir / 'project')
-        expected = (
-            'git clone --quiet '
-            'https://github.example/group/project {expected_dir}'
-        ).format(**locals())
-        os_system_mock.assert_called_once_with(expected)
+        check_call.assert_called_once_with([
+            'git', 'clone', '--quiet',
+            'https://github.example/group/project', expected_dir,
+        ])
+        assert result == expected_dir

     def test_download_svn(self, tmpdir):
         url = 'svn+https://svn.example/project#egg=foo'
         index = setuptools.package_index.PackageIndex()

         with pytest.warns(UserWarning):
-            with mock.patch("os.system") as os_system_mock:
+            with mock.patch("subprocess.check_call") as check_call:
                 result = index.download(url, str(tmpdir))

-        os_system_mock.assert_called()
-
         expected_dir = str(tmpdir / 'project')
-        expected = (
-            'svn checkout -q '
-            'svn+https://svn.example/project {expected_dir}'
-        ).format(**locals())
-        os_system_mock.assert_called_once_with(expected)
+        check_call.assert_called_once_with([
+            'svn', 'checkout', '-q', 'svn+https://svn.example/project', expected_dir,
+        ])
+        assert result == expected_dir


 class TestContentCheckers:
