--- compiler-rt/test/tsan/deadlock_detector_stress_test.cpp.orig
+++ compiler-rt/test/tsan/deadlock_detector_stress_test.cpp
@@ -159,16 +159,16 @@
     // CHECK: WARNING: ThreadSanitizer: lock-order-inversion (potential deadlock)
     // CHECK: Cycle in lock order graph: [[M1:M[0-9]+]] ([[A1]]) => [[M2:M[0-9]+]] ([[A2]]) => [[M1]]
     // CHECK: Mutex [[M2]] acquired here while holding mutex [[M1]]
-    // CHECK:   #0 pthread_
+    // CHECK:   #0 {{_?}}pthread_
     // CHECK-SECOND:   Mutex [[M1]] previously acquired by the same thread here:
-    // CHECK-SECOND:   #0 pthread_
+    // CHECK-SECOND:   #0 {{_?}}pthread_
     // CHECK-NOT-SECOND:   second_deadlock_stack=1 to get more informative warning message
-    // CHECK-NOT-SECOND-NOT:   #0 pthread_
+    // CHECK-NOT-SECOND-NOT:   #0 {{_?}}pthread_
     // CHECK: Mutex [[M1]] acquired here while holding mutex [[M2]]
-    // CHECK:   #0 pthread_
+    // CHECK:   #0 {{_?}}pthread_
     // CHECK-SECOND:   Mutex [[M2]] previously acquired by the same thread here:
-    // CHECK-SECOND:   #0 pthread_
-    // CHECK-NOT-SECOND-NOT:   #0 pthread_
+    // CHECK-SECOND:   #0 {{_?}}pthread_
+    // CHECK-NOT-SECOND-NOT:   #0 {{_?}}pthread_
     // CHECK-NOT: WARNING: ThreadSanitizer:
   }

