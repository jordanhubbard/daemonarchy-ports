--- compiler-rt/test/tsan/mutexset5.cpp.orig
+++ compiler-rt/test/tsan/mutexset5.cpp
@@ -29,10 +29,10 @@
   // CHECK:   Previous write of size 4 at {{.*}} by thread T2
   // CHECK:                              (mutexes: write [[M2:M[0-9]+]]):
   // CHECK:   Mutex [[M1]] (0x{{.*}}) created at:
-  // CHECK:     #0 pthread_mutex_init
+  // CHECK:     #0 {{_?}}pthread_mutex_init
   // CHECK:     #1 main {{.*}}mutexset5.cpp:[[@LINE+4]]
   // CHECK:   Mutex [[M2]] (0x{{.*}}) created at:
-  // CHECK:     #0 pthread_mutex_init
+  // CHECK:     #0 {{_?}}pthread_mutex_init
   // CHECK:     #1 main {{.*}}mutexset5.cpp:[[@LINE+2]]
   pthread_mutex_init(&mtx1, 0);
   pthread_mutex_init(&mtx2, 0);
