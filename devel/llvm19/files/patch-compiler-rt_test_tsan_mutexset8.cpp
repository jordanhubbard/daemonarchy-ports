--- compiler-rt/test/tsan/mutexset8.cpp.orig
+++ compiler-rt/test/tsan/mutexset8.cpp
@@ -25,7 +25,7 @@
   // CHECK:                         (mutexes: write [[M1:M[0-9]+]]):
   // CHECK:   Previous write of size 4 at {{.*}} by thread T2:
   // CHECK:   Mutex [[M1]] (0x{{.*}}) created at:
-  // CHECK:     #0 pthread_mutex_init
+  // CHECK:     #0 {{_?}}pthread_mutex_init
   // CHECK:     #1 main {{.*}}mutexset8.cpp
   mtx = new pthread_mutex_t;
   pthread_mutex_init(mtx, 0);
