--- setup3lib.py.orig
+++ setup3lib.py
@@ -18,7 +18,36 @@
     import logging
     from setuptools import Distribution as _Distribution
     from distutils.core import Command
-    from setuptools.command.build_py import Mixin2to3
+    from setuptools.command.build_py import build_py as _BuildPy
+    from distutils.errors import DistutilsError
+
+    _BuildPyCommand = None
+    try:
+        from setuptools.command.build_py import Mixin2to3
+    except ImportError:
+        # Newer setuptools removed its conversion mixin. Keep conversion in
+        # the original commands, using Python's retained standard-library tool.
+        class Mixin2to3:
+            def run_2to3(self, files, doctests=False):
+                from lib2to3.refactor import RefactoringTool, get_fixers_from_package
+
+                class CheckedRefactoringTool(RefactoringTool):
+                    def log_error(self, message, *args, **kwargs):
+                        raise DistutilsError(message % args)
+
+                fixers = get_fixers_from_package('lib2to3.fixes')
+                for package in getattr(self.distribution, 'use_2to3_fixers', []):
+                    fixers.extend(get_fixers_from_package(package))
+                tool = CheckedRefactoringTool(fixers)
+                tool.refactor(list(files), write=True, doctests_only=doctests)
+
+        class _BuildPyCommand(_BuildPy, Mixin2to3):
+            def byte_compile(self, files):
+                files = list(files)
+                if self.distribution.use_2to3:
+                    self.run_2to3(files)
+                    self.run_2to3(self.distribution.convert_2to3_doctests, True)
+                return _BuildPy.byte_compile(self, files)
     from distutils import dir_util, file_util, log
     import setuptools.command.test
     from pkg_resources import normalize_path
@@ -66,7 +95,17 @@
             self.test_build_dir = None
             self.doctest_exts = ['.py', '.rst']
             self.pyversion_patching = False
+            conversion = {}
+            if _BuildPyCommand is not None:
+                attrs = dict(attrs or {})
+                for key, default in [('use_2to3', False),
+                                     ('convert_2to3_doctests', []),
+                                     ('use_2to3_fixers', [])]:
+                    conversion[key] = attrs.pop(key, default)
             _Distribution.__init__(self, attrs)
+            # Set these after setuptools validates its own setup keywords.
+            for key, value in conversion.items():
+                setattr(self, key, value)
 
     class BuildTestsCommand (Command, Mixin2to3):
         # Create mirror copy of tests, convert all .py files using 2to3
@@ -135,6 +174,8 @@
     def setup(*args, **kwargs):
         kwargs.setdefault('distclass', Distribution)
         cmdclass = kwargs.setdefault('cmdclass', {})
+        if _BuildPyCommand is not None:
+            cmdclass.setdefault('build_py', _BuildPyCommand)
         cmdclass.setdefault('build_tests', BuildTestsCommand)
         cmdclass.setdefault('test', TestCommand)
         return _setup(*args, **kwargs)
