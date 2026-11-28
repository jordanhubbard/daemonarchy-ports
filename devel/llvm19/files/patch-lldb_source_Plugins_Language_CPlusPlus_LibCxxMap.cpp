--- lldb/source/Plugins/Language/CPlusPlus/LibCxxMap.cpp.orig
+++ lldb/source/Plugins/Language/CPlusPlus/LibCxxMap.cpp
@@ -262,11 +262,10 @@
   if (m_tree == nullptr)
     return 0;
 
-  ValueObjectSP size_node(m_tree->GetChildMemberWithName("__pair3_"));
-  if (!size_node)
-    return 0;
-
-  size_node = GetFirstValueOfLibCXXCompressedPair(*size_node);
+  auto [size_node, is_compressed_pair] =
+      GetValueOrOldCompressedPair(*m_tree, "__size_", "__pair3_");
+  if (is_compressed_pair)
+    size_node = GetFirstValueOfLibCXXCompressedPair(*size_node);
 
   if (!size_node)
     return 0;
