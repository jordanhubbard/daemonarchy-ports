--- compiler-rt/test/tsan/mutex_lock_destroyed.cpp.orig
+++ compiler-rt/test/tsan/mutex_lock_destroyed.cpp
@@ -21,7 +21,7 @@
 
   pthread_mutex_lock(m);
   // CHECK: WARNING: ThreadSanitizer: use of an invalid mutex (e.g. uninitialized or destroyed)
-  // CHECK:   #0 pthread_mutex_lock
+  // CHECK:   #0 {{_?}}pthread_mutex_lock
   // CHECK:   #1 main {{.*}}mutex_lock_destroyed.cpp:[[@LINE-3]]
 
   return 0;
