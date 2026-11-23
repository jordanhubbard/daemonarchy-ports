--- compiler-rt/test/tsan/race_on_mutex.c.orig
+++ compiler-rt/test/tsan/race_on_mutex.c
@@ -8,7 +8,7 @@
   barrier_wait(&barrier);
 // CHECK:      WARNING: ThreadSanitizer: data race
 // CHECK-NEXT:   Atomic read of size 1 at {{.*}} by thread T2:
-// CHECK-NEXT:     #0 pthread_mutex_lock
+// CHECK-NEXT:     #0 {{_?}}pthread_mutex_lock
 // CHECK-NEXT:     #1 Thread2{{.*}} {{.*}}race_on_mutex.c:[[@LINE+1]]{{(:3)?}} ({{.*}})
   pthread_mutex_lock(&Mtx);
   Global = 43;
