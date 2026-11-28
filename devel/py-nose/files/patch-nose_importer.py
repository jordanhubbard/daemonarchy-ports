--- nose/importer.py.orig
+++ nose/importer.py
@@ -9,7 +9,36 @@
 import sys
 from nose.config import Config
 
-from imp import find_module, load_module, acquire_lock, release_lock
+try:
+    from imp import find_module, load_module, acquire_lock, release_lock
+except ImportError:
+    # Python 3.12 removed imp. Keep path-local lookup and the import lock;
+    # import_module would reuse an unrelated module with the same name.
+    from _imp import acquire_lock, release_lock
+    from importlib.machinery import PathFinder
+    from importlib.util import module_from_spec, spec_from_file_location
+
+    def find_module(name, path):
+        spec = PathFinder.find_spec(name, path)
+        if spec is None or spec.loader is None:
+            raise ImportError("No module named %s" % name)
+        filename = spec.origin
+        if spec.submodule_search_locations is not None:
+            filename = os.path.dirname(filename)
+        return None, filename, spec
+
+    def load_module(name, file, filename, description):
+        spec = spec_from_file_location(
+            name, description.origin,
+            submodule_search_locations=description.submodule_search_locations)
+        module = module_from_spec(spec)
+        sys.modules[name] = module
+        try:
+            spec.loader.exec_module(module)
+        except BaseException:
+            sys.modules.pop(name, None)
+            raise
+        return sys.modules[name]
 
 log = logging.getLogger(__name__)
 
