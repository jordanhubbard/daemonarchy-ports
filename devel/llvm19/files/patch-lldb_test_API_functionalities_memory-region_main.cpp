--- lldb/test/API/functionalities/memory-region/main.cpp.orig
+++ lldb/test/API/functionalities/memory-region/main.cpp
@@ -1,6 +1,37 @@
+#if defined(TEST_MEMORY_REGION_REFRESH)
+#include <sys/mman.h>
+#include <unistd.h>
+#include <stddef.h>
+extern "C" {
+void *test_region = nullptr;
+size_t test_region_size = 0;
+__attribute__((noinline)) void stop_none() { asm volatile("" ::: "memory"); }
+__attribute__((noinline)) void stop_rw() { asm volatile("" ::: "memory"); }
+__attribute__((noinline)) void stop_ro() { asm volatile("" ::: "memory"); }
+__attribute__((noinline)) void stop_unmapped() { asm volatile("" ::: "memory"); }
+}
+int main() {
+  long page_size = sysconf(_SC_PAGESIZE);
+  if (page_size <= 0) return 1;
+  test_region_size = static_cast<size_t>(page_size);
+  test_region = mmap(nullptr, test_region_size, PROT_NONE,
+                     MAP_PRIVATE | MAP_ANON, -1, 0);
+  if (test_region == MAP_FAILED) return 2;
+  stop_none();
+  if (mprotect(test_region, test_region_size, PROT_READ | PROT_WRITE)) return 3;
+  *static_cast<volatile unsigned char *>(test_region) = 42;
+  stop_rw();
+  if (mprotect(test_region, test_region_size, PROT_READ)) return 4;
+  stop_ro();
+  if (munmap(test_region, test_region_size)) return 5;
+  stop_unmapped();
+  return 0;
+}
+#else
 #include <iostream>
 
 int main (int argc, char const **argv) {
   std::cout << "Program with sections" << std::endl;
   return 0; // Run here before printing memory regions
 }
+#endif
