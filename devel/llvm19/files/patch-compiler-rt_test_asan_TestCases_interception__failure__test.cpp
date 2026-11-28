--- compiler-rt/test/asan/TestCases/interception_failure_test.cpp.orig
+++ compiler-rt/test/asan/TestCases/interception_failure_test.cpp
@@ -5,7 +5,7 @@
 // RUN: %clangxx_asan -O1 %s -o %t && %run %t 2>&1 | FileCheck %s
 // RUN: %clangxx_asan -O2 %s -o %t && %run %t 2>&1 | FileCheck %s
 // RUN: %clangxx_asan -O3 %s -o %t && %run %t 2>&1 | FileCheck %s
-// XFAIL: target={{.*freebsd.*}}
+// XFAIL: target={{.*freebsd.*}} && !asan-dynamic-runtime
 
 // On Windows, the static runtime build _will_ intercept static copies of libc
 // functions, making this test invalid.
