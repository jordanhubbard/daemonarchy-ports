--- compiler-rt/test/tsan/mutex_destroy_locked.cpp.orig
+++ compiler-rt/test/tsan/mutex_destroy_locked.cpp
@@ -11,12 +11,12 @@
 }
 
 // CHECK: WARNING: ThreadSanitizer: destroy of a locked mutex
-// CHECK:     #0 pthread_mutex_destroy
+// CHECK:     #0 {{_?}}pthread_mutex_destroy
 // CHECK:     #1 main
 // CHECK:   and:
-// CHECK:     #0 pthread_mutex_lock
+// CHECK:     #0 {{_?}}pthread_mutex_lock
 // CHECK:     #1 main
 // CHECK:   Mutex {{.*}} created at:
-// CHECK:     #0 pthread_mutex_init
+// CHECK:     #0 {{_?}}pthread_mutex_init
 // CHECK:     #1 main
 // CHECK: SUMMARY: ThreadSanitizer: destroy of a locked mutex{{.*}}main
