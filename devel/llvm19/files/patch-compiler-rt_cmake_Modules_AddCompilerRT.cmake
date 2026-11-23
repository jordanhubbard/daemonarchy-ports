--- compiler-rt/cmake/Modules/AddCompilerRT.cmake.orig
+++ compiler-rt/cmake/Modules/AddCompilerRT.cmake
@@ -467,7 +467,8 @@
       )
   endforeach()
 
-  set(TEST_DEPS ${${test_objects}})
+  # Keep declared runtime dependencies on the link step, not only compilation.
+  list(APPEND TEST_DEPS ${${test_objects}})
 
   if(NOT "${TEST_RUNTIME}" STREQUAL "")
     list(APPEND TEST_DEPS ${TEST_RUNTIME})
