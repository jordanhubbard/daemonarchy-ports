--- compiler-rt/test/sanitizer_common/TestCases/FreeBSD/capsicum.cpp.orig
+++ compiler-rt/test/sanitizer_common/TestCases/FreeBSD/capsicum.cpp
@@ -13,7 +13,7 @@
 void test_cap_ioctls() {
   cap_rights_t rights;
   unsigned long ncmds[] = {TIOCGETA, TIOCGWINSZ, FIODTYPE};
-  unsigned long rcmds = 0;
+  unsigned long rcmds[3];
   cap_rights_t *rptr = cap_rights_init(&rights, CAP_IOCTL, CAP_READ);
   assert(rptr);
 
@@ -21,7 +21,7 @@
   assert(rv == 0);
   rv = cap_ioctls_limit(STDIN_FILENO, ncmds, 3);
   assert(rv == 0);
-  ssize_t rz = cap_ioctls_get(STDIN_FILENO, &rcmds, 3);
+  ssize_t rz = cap_ioctls_get(STDIN_FILENO, rcmds, 3);
   assert(rz == 3);
   printf("ioctls test: %ld commands authorized\n", rz);
 }
