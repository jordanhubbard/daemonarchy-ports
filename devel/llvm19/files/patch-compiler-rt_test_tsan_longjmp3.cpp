--- compiler-rt/test/tsan/longjmp3.cpp.orig
+++ compiler-rt/test/tsan/longjmp3.cpp
@@ -42,7 +42,7 @@
 
 // CHECK-NOT: FAILED
 // CHECK: WARNING: ThreadSanitizer: destroy of a locked mutex
-// CHECK:   #0 pthread_mutex_destroy
+// CHECK:   #0 {{_?}}pthread_mutex_destroy
 // CHECK:   #1 badguy
 // CHECK:   #2 mymain
 // CHECK:   #3 main
