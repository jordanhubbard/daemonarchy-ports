--- lldb/source/Plugins/Language/CPlusPlus/LibCxxUnorderedMap.cpp.orig
+++ lldb/source/Plugins/Language/CPlusPlus/LibCxxUnorderedMap.cpp
@@ -136,11 +136,13 @@
     ValueObjectSP hash_sp = node_sp->GetChildMemberWithName("__hash_");
     if (!hash_sp || !value_sp) {
       if (!m_element_type) {
-        auto p1_sp = m_backend.GetChildAtNamePath({"__table_", "__p1_"});
-        if (!p1_sp)
-          return nullptr;
-
-        ValueObjectSP first_sp = GetFirstValueOfLibCXXCompressedPair(*p1_sp);
+        auto table_sp = m_backend.GetChildMemberWithName("__table_");
+        if (!table_sp)
+          return nullptr;
+        auto [first_sp, is_compressed_pair] =
+            GetValueOrOldCompressedPair(*table_sp, "__first_node_", "__p1_");
+        if (is_compressed_pair)
+          first_sp = GetFirstValueOfLibCXXCompressedPair(*first_sp);
         if (!first_sp)
           return nullptr;
 
@@ -149,6 +151,12 @@
         m_element_type = m_element_type.GetPointeeType();
         m_node_type = m_element_type;
         m_element_type = m_element_type.GetTypeTemplateArgument(0);
+        CompilerType table_element_type =
+            table_sp->GetCompilerType()
+                .GetDirectNestedTypeWithName("value_type")
+                .GetTypedefedType();
+        if (table_element_type)
+          m_element_type = table_element_type;
         // This synthetic provider is used for both unordered_(multi)map and
         // unordered_(multi)set. For unordered_map, the element type has an
         // additional type layer, an internal struct (`__hash_value_type`)
@@ -226,19 +234,17 @@
   if (!table_sp)
     return lldb::ChildCacheState::eRefetch;
 
-  ValueObjectSP p2_sp = table_sp->GetChildMemberWithName("__p2_");
-  if (!p2_sp)
-    return lldb::ChildCacheState::eRefetch;
-
-  ValueObjectSP num_elements_sp = GetFirstValueOfLibCXXCompressedPair(*p2_sp);
+  auto [num_elements_sp, size_is_compressed_pair] =
+      GetValueOrOldCompressedPair(*table_sp, "__size_", "__p2_");
+  if (size_is_compressed_pair)
+    num_elements_sp = GetFirstValueOfLibCXXCompressedPair(*num_elements_sp);
   if (!num_elements_sp)
     return lldb::ChildCacheState::eRefetch;
 
-  ValueObjectSP p1_sp = table_sp->GetChildMemberWithName("__p1_");
-  if (!p1_sp)
-    return lldb::ChildCacheState::eRefetch;
-
-  ValueObjectSP value_sp = GetFirstValueOfLibCXXCompressedPair(*p1_sp);
+  auto [value_sp, node_is_compressed_pair] =
+      GetValueOrOldCompressedPair(*table_sp, "__first_node_", "__p1_");
+  if (node_is_compressed_pair)
+    value_sp = GetFirstValueOfLibCXXCompressedPair(*value_sp);
   if (!value_sp)
     return lldb::ChildCacheState::eRefetch;
 
