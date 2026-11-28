--- compiler-rt/test/sanitizer_common/TestCases/Posix/huge_malloc.c.orig
+++ compiler-rt/test/sanitizer_common/TestCases/Posix/huge_malloc.c
@@ -1,5 +1,8 @@
-// RUN: %clang -O0 %s -o %t && %env_tool_opts=allocator_may_return_null=1:hard_rss_limit_mb=50:quarantine_size_mb=1 %run %t
+// RUN: %clang -O0 %s -o %t && %env_tool_opts=allocator_may_return_null=1:hard_rss_limit_mb=50:quarantine_size_mb=1 env MALLOC_CONF=junk:false %run %t
+// Debug jemalloc (the FreeBSD CURRENT default) junk-fills freed blocks.
+// This test relies on huge calloc allocations remaining virtual. Disable
+// that unrelated filling only for this process, retaining all size probes.

 #include <stdlib.h>
 #include <stdio.h>

