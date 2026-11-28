--- compiler-rt/test/tsan/mutexset7.cpp.orig
+++ compiler-rt/test/tsan/mutexset7.cpp
@@ -37,5 +37,5 @@
 // CHECK: Previous write of size 4 at {{.*}} by thread T2
 // CHECK:                                      (mutexes: write [[M1:M[0-9]+]]):
 // CHECK: Mutex [[M1]] (0x{{.*}}) created at:
-// CHECK:   #0 pthread_mutex_init
+// CHECK:   #0 {{_?}}pthread_mutex_init
 // CHECK:   #1 Thread2
