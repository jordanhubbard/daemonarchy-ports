--- lldb/source/Plugins/Language/CPlusPlus/LibCxxList.cpp.orig
+++ lldb/source/Plugins/Language/CPlusPlus/LibCxxList.cpp
@@ -292,10 +292,16 @@
   if (err.Fail() || !backend_addr)
     return lldb::ChildCacheState::eRefetch;
 
-  ValueObjectSP impl_sp(m_backend.GetChildMemberWithName("__before_begin_"));
-  if (!impl_sp)
-    return lldb::ChildCacheState::eRefetch;
-  impl_sp = GetFirstValueOfLibCXXCompressedPair(*impl_sp);
+  auto impl = GetValueOrOldCompressedPair(m_backend, "__before_begin_",
+                                         "__before_begin_");
+  // New anonymous members may be reached only through the list's base class.
+  if (!impl.first)
+    if (ValueObjectSP base_sp = m_backend.GetChildAtIndex(0))
+      impl = GetValueOrOldCompressedPair(*base_sp, "__before_begin_",
+                                        "__before_begin_");
+  ValueObjectSP impl_sp = impl.first;
+  if (impl.second)
+    impl_sp = GetFirstValueOfLibCXXCompressedPair(*impl_sp);
   if (!impl_sp)
     return lldb::ChildCacheState::eRefetch;
   m_head = impl_sp->GetChildMemberWithName("__next_").get();
@@ -313,13 +319,12 @@
     return m_count;
   if (!m_head || !m_tail || m_node_address == 0)
     return 0;
-  ValueObjectSP size_alloc(m_backend.GetChildMemberWithName("__size_alloc_"));
-  if (size_alloc) {
-    ValueObjectSP value = GetFirstValueOfLibCXXCompressedPair(*size_alloc);
-    if (value) {
-      m_count = value->GetValueAsUnsigned(UINT32_MAX);
-    }
-  }
+  auto [value, is_compressed_pair] =
+      GetValueOrOldCompressedPair(m_backend, "__size_", "__size_alloc_");
+  if (is_compressed_pair)
+    value = GetFirstValueOfLibCXXCompressedPair(*value);
+  if (value)
+    m_count = value->GetValueAsUnsigned(UINT32_MAX);
   if (m_count != UINT32_MAX) {
     return m_count;
   } else {
