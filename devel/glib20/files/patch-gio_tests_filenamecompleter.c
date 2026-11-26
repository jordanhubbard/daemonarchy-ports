Backport the test-only portion of the upstream unspecified-order correction.
Expected members, multiplicity, directory-only and suffix checks are retained.

Obtained from: https://github.com/GNOME/glib/commit/70355ac696600d7cd99dcb7e1cf110b52bd158b9

--- gio/tests/filenamecompleter.c.orig
+++ gio/tests/filenamecompleter.c
@@ -45,6 +45,23 @@
   return g_strv_builder_unref_to_strv (builder);
 }

+static int
+strcmp_data (const void *a,
+             const void *b,
+             void       *user_data)
+{
+  return strcmp (*((const char * const *) a), *((const char * const *) b));
+}
+
+#define assert_cmpstrv_unsorted(a1, a2) \
+  G_STMT_START { \
+    const char **_a1 = (const char **) (a1); \
+    const char **_a2 = (const char **) (a2); \
+    g_sort_array (_a1, g_strv_length ((char **) _a1), sizeof (*_a1), strcmp_data, NULL); \
+    g_sort_array (_a2, g_strv_length ((char **) _a2), sizeof (*_a2), strcmp_data, NULL); \
+    g_assert_cmpstrv (_a1, _a2); \
+  } G_STMT_END
+
 typedef struct
 {
   const gchar *string;
@@ -80,7 +97,7 @@

       all_expected_completions = prefix_filenames (test_cases[i].all_completions, base_path);
       all_results_completions = g_filename_completer_get_completions (all_completer, path_to_complete);
-      g_assert_cmpstrv (all_expected_completions, all_results_completions);
+      assert_cmpstrv_unsorted (all_expected_completions, all_results_completions);
       g_strfreev (all_expected_completions);
       g_strfreev (all_results_completions);

@@ -99,7 +116,7 @@

       dirs_expected_completions = prefix_filenames (test_cases[i].dirs_completions, base_path);
       dirs_results_completions = g_filename_completer_get_completions (dirs_completer, path_to_complete);
-      g_assert_cmpstrv (dirs_expected_completions, dirs_results_completions);
+      assert_cmpstrv_unsorted (dirs_expected_completions, dirs_results_completions);

       g_strfreev (dirs_expected_completions);
       g_strfreev (dirs_results_completions);
