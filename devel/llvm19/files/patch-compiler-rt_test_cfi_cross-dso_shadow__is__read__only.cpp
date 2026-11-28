--- compiler-rt/test/cfi/cross-dso/shadow_is_read_only.cpp.orig
+++ compiler-rt/test/cfi/cross-dso/shadow_is_read_only.cpp
@@ -66,7 +66,7 @@
 
   if (test_mmap) {
     void *p = mmap(nullptr, 1 << 20, PROT_READ | PROT_WRITE | PROT_EXEC,
-                   MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
+                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
     assert(p != MAP_FAILED);
     write_shadow((char *)p + 100);
   } else {
