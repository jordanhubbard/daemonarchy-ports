--- lldb/test/API/functionalities/data-formatter/data-formatter-stl/libcxx-simulators/unique_ptr/main.cpp.orig
+++ lldb/test/API/functionalities/data-formatter/data-formatter-stl/libcxx-simulators/unique_ptr/main.cpp
@@ -16,9 +16,14 @@
   typedef _Dp deleter_type;
   typedef _Tp *pointer;
 
+#if COMPRESSED_PAIR_REV == 0
   std::__lldb::__compressed_pair<pointer, deleter_type> __ptr_;
   explicit unique_ptr(pointer __p) noexcept
       : __ptr_(__p, std::__lldb::__value_init_tag()) {}
+#else
+  _LLDB_COMPRESSED_PAIR(pointer, __ptr_, deleter_type, __deleter_);
+  explicit unique_ptr(pointer __p) noexcept : __ptr_(__p), __deleter_() {}
+#endif
 };
 } // namespace __lldb
 } // namespace std
