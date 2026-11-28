--- lldb/examples/synthetic/libcxx.py.orig
+++ lldb/examples/synthetic/libcxx.py
@@ -72,6 +72,58 @@
             return "<error:" + error.GetCString() + ">"
         else:
             return '"' + strval + '"'
+
+
+def get_buffer_end(buffer, begin):
+    """
+    Returns a pointer to where the next element would be pushed.
+
+    For libc++'s stable ABI and unstable < LLVM 22, returns `__end_`.
+    For libc++'s unstable ABI, returns `__begin_ + __size_`.
+    """
+    map_end = buffer.GetChildMemberWithName("__end_")
+    if map_end.IsValid():
+        return map_end.GetValueAsUnsigned(0)
+    map_size = buffer.GetChildMemberWithName("__size_").GetValueAsUnsigned(0)
+    return begin + map_size
+
+def get_buffer_endcap(parent, buffer, begin, has_compressed_pair_layout, is_size_based):
+    """
+    Returns a pointer to the end of the buffer.
+
+    For libc++'s stable ABI and unstable < LLVM 22, returns:
+        * `__end_cap_`, if `__compressed_pair` is being used
+        * `__cap_`, otherwise
+    For libc++'s unstable ABI, returns `__begin_ + __cap_`.
+    """
+    if has_compressed_pair_layout:
+        map_endcap = parent._get_value_of_compressed_pair(
+            buffer.GetChildMemberWithName("__end_cap_")
+        )
+    elif buffer.GetType().GetNumberOfDirectBaseClasses() == 1:
+        # LLVM 22's __split_buffer is derived from a base class that describes its layout. When the
+        # compressed pair ABI is required, we also use an anonymous struct. Per [#158131], LLDB
+        # is unable to access members of an anonymous struct to a base class, through the derived
+        # class. This means that in order to access the compressed pair's pointer, we need to first
+        # get to its base class.
+        #
+        # [#158131]: https://github.com/llvm/llvm-project/issues/158131
+        buffer = buffer.GetChildAtIndex(0)
+        if is_size_based:
+            map_endcap = buffer.GetChildMemberWithName("__cap_")
+        else:
+            map_endcap = buffer.GetChildMemberWithName("__back_cap_")
+        map_endcap = map_endcap.GetValueAsUnsigned(0)
+    else:
+        map_endcap = buffer.GetChildMemberWithName("__cap_")
+        if not map_endcap.IsValid():
+            map_endcap = buffer.GetChildMemberWithName("__end_cap_")
+        map_endcap = map_endcap.GetValueAsUnsigned(0)
+
+    if is_size_based:
+        return begin + map_endcap
+
+    return map_endcap
 
 
 class stdvector_SynthProvider:
@@ -721,6 +773,12 @@
     def update(self):
         logger = lldb.formatters.Logger.Logger()
         try:
+            has_compressed_pair_layout = True
+            alloc_valobj = self.valobj.GetChildMemberWithName("__alloc_")
+            size_valobj = self.valobj.GetChildMemberWithName("__size_")
+            if alloc_valobj.IsValid() and size_valobj.IsValid():
+                has_compressed_pair_layout = False
+
             # A deque is effectively a two-dim array, with fixed width.
             # 'map' contains pointers to the rows of this array. The
             # full memory area allocated by the deque is delimited
@@ -734,22 +792,31 @@
             # variable tells which element in this NxM array is the 0th
             # one, and the 'size' element gives the number of elements
             # in the deque.
-            count = self._get_value_of_compressed_pair(
-                self.valobj.GetChildMemberWithName("__size_")
-            )
+            if has_compressed_pair_layout:
+                count = self._get_value_of_compressed_pair(
+                    self.valobj.GetChildMemberWithName("__size_")
+                )
+            else:
+                count = size_valobj.GetValueAsUnsigned(0)
+
             # give up now if we cant access memory reliably
             if self.block_size < 0:
                 logger.write("block_size < 0")
                 return
+            start = self.valobj.GetChildMemberWithName("__start_").GetValueAsUnsigned(0)
+
             map_ = self.valobj.GetChildMemberWithName("__map_")
-            start = self.valobj.GetChildMemberWithName("__start_").GetValueAsUnsigned(0)
+            is_size_based = map_.GetChildMemberWithName("__size_").IsValid()
             first = map_.GetChildMemberWithName("__first_")
+            # LLVM 22 renames __map_.__begin_ to __map_.__front_cap_
+            if not first:
+                first = map_.GetChildMemberWithName("__front_cap_")
             map_first = first.GetValueAsUnsigned(0)
             self.map_begin = map_.GetChildMemberWithName("__begin_")
             map_begin = self.map_begin.GetValueAsUnsigned(0)
-            map_end = map_.GetChildMemberWithName("__end_").GetValueAsUnsigned(0)
-            map_endcap = self._get_value_of_compressed_pair(
-                map_.GetChildMemberWithName("__end_cap_")
+            map_end = get_buffer_end(map_, map_begin)
+            map_endcap = get_buffer_endcap(
+                self, map_, map_begin, has_compressed_pair_layout, is_size_based
             )
 
             # check consistency
