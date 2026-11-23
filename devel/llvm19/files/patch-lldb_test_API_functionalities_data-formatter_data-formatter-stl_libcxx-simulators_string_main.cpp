--- lldb/test/API/functionalities/data-formatter/data-formatter-stl/libcxx-simulators/string/main.cpp.orig
+++ lldb/test/API/functionalities/data-formatter/data-formatter-stl/libcxx-simulators/string/main.cpp
@@ -20,7 +20,11 @@
 // Pre-D128285 layout.
 #define PACKED_ANON_STRUCT
 #endif
-// REVISION == 4: current layout
+#if REVISION <= 4
+// Pre-2a1ef74 layout.
+#define NON_STANDARD_PADDING
+#endif
+// REVISION == 5: current layout
 
 #ifdef PACKED_ANON_STRUCT
 #define BEGIN_PACKED_ANON_STRUCT struct __attribute__((packed)) {
@@ -34,12 +38,20 @@
 namespace std {
 namespace __lldb {
 
+#ifdef NON_STANDARD_PADDING
 #if defined(ALTERNATE_LAYOUT) && defined(SUBCLASS_PADDING)
 template <class _CharT, size_t = sizeof(_CharT)> struct __padding {
   unsigned char __xx[sizeof(_CharT) - 1];
 };
 
 template <class _CharT> struct __padding<_CharT, 1> {};
+#endif
+#else // !NON_STANDARD_PADDING
+template <size_t _PaddingSize> struct __padding {
+  char __padding_[_PaddingSize];
+};
+
+template <> struct __padding<0> {};
 #endif
 
 template <class _CharT, class _Traits, class _Allocator> class basic_string {
@@ -71,19 +83,25 @@
 
   struct __short {
     value_type __data_[__min_cap];
-#ifdef BITMASKS
 #ifdef SUBCLASS_PADDING
     struct : __padding<value_type> {
       unsigned char __size_;
     };
-#else
+#else // !SUBCLASS_PADDING
+
+#ifdef NON_STANDARD_PADDING
     unsigned char __padding[sizeof(value_type) - 1];
+#else
+    _LLDB_NO_UNIQUE_ADDRESS __padding<sizeof(value_type) - 1> __padding_;
+#endif
+
+#ifdef BITMASKS
     unsigned char __size_;
-#endif
 #else // !BITMASKS
     unsigned char __size_ : 7;
     unsigned char __is_long_ : 1;
-#endif
+#endif // BITMASKS
+#endif // SUBCLASS_PADDING
   };
 
 #ifdef BITMASKS
@@ -128,21 +146,26 @@
     union {
 #ifdef BITMASKS
       unsigned char __size_;
-#else
+#else  // !BITMASKS
       struct {
         unsigned char __is_long_ : 1;
         unsigned char __size_ : 7;
       };
-#endif
+#endif // BITMASKS
       value_type __lx;
     };
-#else
+#else  // !SHORT_UNION
     BEGIN_PACKED_ANON_STRUCT
     unsigned char __is_long_ : 1;
     unsigned char __size_ : 7;
     END_PACKED_ANON_STRUCT
-    char __padding_[sizeof(value_type) - 1];
-#endif
+#ifdef NON_STANDARD_PADDING
+    unsigned char __padding[sizeof(value_type) - 1];
+#else  // !NON_STANDARD_PADDING
+    _LLDB_NO_UNIQUE_ADDRESS __padding<sizeof(value_type) - 1> __padding_;
+#endif // NON_STANDARD_PADDING
+
+#endif // SHORT_UNION
     value_type __data_[__min_cap];
   };
 
@@ -183,31 +206,50 @@
     };
   };
 
+  __long &getLongRep() {
+#if COMPRESSED_PAIR_REV == 0
+    return __r_.first().__l;
+#else
+    return __rep_.__l;
+#endif
+  }
+
+  __short &getShortRep() {
+#if COMPRESSED_PAIR_REV == 0
+    return __r_.first().__s;
+#else
+    return __rep_.__s;
+#endif
+  }
+
+#if COMPRESSED_PAIR_REV == 0
   std::__lldb::__compressed_pair<__rep, allocator_type> __r_;
+#else
+  _LLDB_COMPRESSED_PAIR(__rep, __rep_, allocator_type, __alloc_);
+#endif
 
 public:
   template <size_t __N>
-  basic_string(unsigned char __size, const value_type (&__data)[__N])
-      : __r_({}, {}) {
+  basic_string(unsigned char __size, const value_type (&__data)[__N]) {
     static_assert(__N < __min_cap, "");
 #ifdef BITMASKS
-    __r_.first().__s.__size_ = __size << __short_shift;
-#else
-    __r_.first().__s.__size_ = __size;
-    __r_.first().__s.__is_long_ = false;
+    getShortRep().__size_ = __size << __short_shift;
+#else
+    getShortRep().__size_ = __size;
+    getShortRep().__is_long_ = false;
 #endif
     for (size_t __i = 0; __i < __N; ++__i)
-      __r_.first().__s.__data_[__i] = __data[__i];
-  }
-  basic_string(size_t __cap, size_type __size, pointer __data) : __r_({}, {}) {
-#ifdef BITMASKS
-    __r_.first().__l.__cap_ = __cap | __long_mask;
-#else
-    __r_.first().__l.__cap_ = __cap / __endian_factor;
-    __r_.first().__l.__is_long_ = true;
-#endif
-    __r_.first().__l.__size_ = __size;
-    __r_.first().__l.__data_ = __data;
+      getShortRep().__data_[__i] = __data[__i];
+  }
+  basic_string(size_t __cap, size_type __size, pointer __data) {
+#ifdef BITMASKS
+    getLongRep().__cap_ = __cap | __long_mask;
+#else
+    getLongRep().__cap_ = __cap / __endian_factor;
+    getLongRep().__is_long_ = true;
+#endif
+    getLongRep().__size_ = __size;
+    getLongRep().__data_ = __data;
   }
 };
 
