--- compiler-rt/test/sanitizer_common/TestCases/FreeBSD/procctl.cpp.orig
+++ compiler-rt/test/sanitizer_common/TestCases/FreeBSD/procctl.cpp
@@ -21,8 +21,11 @@
     assert(errno == EPERM);
     return 0;
   }

-  assert(aslr >= PROC_ASLR_FORCE_ENABLE);
+  // PROC_ASLR_STATUS also reports the active-image flag.
+  aslr &= ~PROC_ASLR_ACTIVE;
+  assert(aslr == PROC_ASLR_FORCE_ENABLE || aslr == PROC_ASLR_FORCE_DISABLE ||
+         aslr == PROC_ASLR_NOFORCE);

   return 0;
 }
