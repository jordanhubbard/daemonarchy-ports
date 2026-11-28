--- compiler-rt/test/tsan/mutex_destroy_locked2.cpp.orig
+++ compiler-rt/test/tsan/mutex_destroy_locked2.cpp
@@ -18,12 +18,12 @@
 }
 
 // CHECK: WARNING: ThreadSanitizer: destroy of a locked mutex
-// CHECK:     #0 pthread_mutex_destroy
+// CHECK:     #0 {{_?}}pthread_mutex_destroy
 // CHECK:     #1 thread
 // CHECK:   and:
-// CHECK:     #0 pthread_mutex_lock
+// CHECK:     #0 {{_?}}pthread_mutex_lock
 // CHECK:     #1 thread
 // CHECK:   Mutex {{.*}} created at:
-// CHECK:     #0 pthread_mutex_init
+// CHECK:     #0 {{_?}}pthread_mutex_init
 // CHECK:     #1 thread
 // CHECK: SUMMARY: ThreadSanitizer: destroy of a locked mutex {{.*}} in thread
