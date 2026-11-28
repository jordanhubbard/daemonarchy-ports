--- openmp/runtime/test/ompt/loadtool/tool_available_search/tool_available_search.c.orig
+++ openmp/runtime/test/ompt/loadtool/tool_available_search/tool_available_search.c
@@ -3,7 +3,7 @@
 // RUN: %clang %flags -DTOOL -DTHIRD_TOOL -shared -fPIC %s -o %T/third_tool.so
 // RUN: %libomp-compile -DCODE
 // RUN: env OMP_TOOL_LIBRARIES=%T/non_existing_file.so:%T/first_tool.so:%T/second_tool.so:%T/third_tool.so \
-// RUN: OMP_TOOL_VERBOSE_INIT=stdout %libomp-run | FileCheck %s -DPARENTPATH=%T
+// RUN: OMP_TOOL_VERBOSE_INIT=stdout %libomp-run | FileCheck %s -DPARENTPATH=%T --check-prefixes=CHECK,%ompt-loader-error-prefix
 
 // REQUIRES: ompt
 // XFAIL: darwin
@@ -26,12 +26,14 @@
 // CHECK-SAME: [[PARENTPATH]]/second_tool.so
 // CHECK-SAME: [[PARENTPATH]]/third_tool.so
 // CHECK-NEXT: Opening [[PARENTPATH]]/non_existing_file.so... Failed:
-// CHECK-SAME: [[PARENTPATH]]/non_existing_file.so: cannot open shared object
-// CHECK-SAME: file: No such file or directory
+// DEFAULT-SAME: [[PARENTPATH]]/non_existing_file.so: cannot open shared object
+// DEFAULT-SAME: file: No such file or directory
+// FREEBSD-SAME: Cannot open "[[PARENTPATH]]/non_existing_file.so"
 // CHECK-NEXT: Opening [[PARENTPATH]]/first_tool.so... Success.
 // CHECK-NEXT: Searching for ompt_start_tool in
 // CHECK-SAME: [[PARENTPATH]]/first_tool.so... Failed:
-// CHECK-SAME: [[PARENTPATH]]/first_tool.so: undefined symbol: ompt_start_tool
+// DEFAULT-SAME: [[PARENTPATH]]/first_tool.so: undefined symbol: ompt_start_tool
+// FREEBSD-SAME: Undefined symbol "ompt_start_tool"
 // CHECK-NEXT: Opening [[PARENTPATH]]/second_tool.so... Success.
 // CHECK-NEXT: Searching for ompt_start_tool in
 // CHECK-SAME: [[PARENTPATH]]/second_tool.so... 0: Do not initialize tool
