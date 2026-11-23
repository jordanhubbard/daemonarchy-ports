--- lldb/source/Plugins/Language/CPlusPlus/LibCxxVector.cpp.orig
+++ lldb/source/Plugins/Language/CPlusPlus/LibCxxVector.cpp
@@ -119,13 +119,11 @@
 lldb::ChildCacheState
 lldb_private::formatters::LibcxxStdVectorSyntheticFrontEnd::Update() {
   m_start = m_finish = nullptr;
-  ValueObjectSP data_type_finder_sp(
-      m_backend.GetChildMemberWithName("__end_cap_"));
-  if (!data_type_finder_sp)
-    return lldb::ChildCacheState::eRefetch;
-
-  data_type_finder_sp =
-      GetFirstValueOfLibCXXCompressedPair(*data_type_finder_sp);
+  auto [data_type_finder_sp, is_compressed_pair] =
+      GetValueOrOldCompressedPair(m_backend, "__cap_", "__end_cap_");
+  if (is_compressed_pair)
+    data_type_finder_sp =
+        GetFirstValueOfLibCXXCompressedPair(*data_type_finder_sp);
   if (!data_type_finder_sp)
     return lldb::ChildCacheState::eRefetch;
 
