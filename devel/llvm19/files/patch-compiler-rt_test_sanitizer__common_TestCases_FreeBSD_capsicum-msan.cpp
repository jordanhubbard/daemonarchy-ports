--- compiler-rt/test/sanitizer_common/TestCases/FreeBSD/capsicum-msan.cpp.orig
+++ compiler-rt/test/sanitizer_common/TestCases/FreeBSD/capsicum-msan.cpp
@@ -0,0 +1,135 @@
+// REQUIRES: msan
+// RUN: %clangxx -O0 -g %s -o %t && %run %t
+// RUN: not %run %t set 2>&1 | FileCheck %s --check-prefix=SET
+// RUN: not %run %t clear 2>&1 | FileCheck %s --check-prefix=CLEAR
+// RUN: not %run %t merge 2>&1 | FileCheck %s --check-prefix=MERGE
+// RUN: not %run %t remove 2>&1 | FileCheck %s --check-prefix=REMOVE
+// RUN: not %run %t limit 2>&1 | FileCheck %s --check-prefix=LIMIT
+
+#include <sys/capsicum.h>
+#include <sys/ioctl.h>
+
+#include <assert.h>
+#include <errno.h>
+#include <fcntl.h>
+#include <sanitizer/msan_interface.h>
+#include <string.h>
+#include <termios.h>
+#include <unistd.h>
+
+static void test_rights() {
+  cap_rights_t rights, other;
+  assert(cap_rights_init(&rights) == &rights);
+  assert(__msan_test_shadow(&rights, sizeof(rights)) == -1);
+  assert(cap_rights_is_set(&rights));
+  assert(!cap_rights_is_set(&rights, CAP_READ));
+
+  // Exercise arguments in registers and on the stack, with duplicates and
+  // rights from both words of the native representation.
+  assert(cap_rights_init(&rights, CAP_READ, CAP_WRITE, CAP_IOCTL, CAP_FSTAT,
+                         CAP_READ, CAP_WRITE, CAP_IOCTL, CAP_FSTAT, CAP_FCNTL,
+                         CAP_EVENT) == &rights);
+  assert(__msan_test_shadow(&rights, sizeof(rights)) == -1);
+  assert(cap_rights_is_set(&rights, CAP_READ, CAP_WRITE, CAP_IOCTL, CAP_FSTAT,
+                          CAP_READ, CAP_WRITE, CAP_IOCTL, CAP_FSTAT, CAP_FCNTL,
+                          CAP_EVENT));
+  assert(cap_rights_clear(&rights, CAP_READ, CAP_WRITE, CAP_READ, CAP_WRITE,
+                          CAP_READ, CAP_WRITE, CAP_READ, CAP_WRITE) == &rights);
+  assert(!cap_rights_is_set(&rights, CAP_READ));
+  // libc stops at the first missing right, before validating later arguments.
+  assert(!cap_rights_is_set(&rights, CAP_READ, ~0ULL));
+  assert(cap_rights_set(&rights, CAP_READ, CAP_WRITE, CAP_READ, CAP_WRITE,
+                        CAP_READ, CAP_WRITE, CAP_READ, CAP_WRITE) == &rights);
+  assert(cap_rights_is_set(&rights, CAP_READ, CAP_WRITE, CAP_IOCTL, CAP_EVENT));
+  assert(cap_rights_init(&other, CAP_READ) == &other);
+  assert(cap_rights_remove(&rights, &other) == &rights);
+  assert(!cap_rights_is_set(&rights, CAP_READ));
+  assert(cap_rights_merge(&rights, &other) == &rights);
+  assert(cap_rights_is_set(&rights, CAP_READ));
+}
+
+static void test_outputs() {
+  int fd = open("/dev/null", O_RDONLY);
+  assert(fd >= 0);
+  cap_rights_t rights;
+  __msan_poison(&rights, sizeof(rights));
+  assert(cap_rights_get(fd, &rights) == 0);
+  assert(__msan_test_shadow(&rights, sizeof(rights)) == -1);
+  __msan_poison(&rights, sizeof(rights));
+  assert(cap_rights_get(-1, &rights) == -1);
+  assert(errno == EBADF);
+  assert(__msan_test_shadow(&rights, sizeof(rights)) == 0);
+
+  unsigned long out[5];
+  __msan_poison(out, sizeof(out));
+  assert(cap_ioctls_get(fd, out, 5) == CAP_IOCTLS_ALL);
+  assert(__msan_test_shadow(out, sizeof(out)) == 0);
+
+  unsigned long allowed[] = {TIOCGETA, TIOCGWINSZ, FIODTYPE};
+  assert(cap_ioctls_limit(fd, allowed, 3) == 0);
+  assert(cap_ioctls_get(fd, out, 5) == 3);
+  assert(__msan_test_shadow(out, sizeof(allowed)) == -1);
+  assert(__msan_test_shadow(out + 3, 2 * sizeof(*out)) == 0);
+  // Compare the returned set without relying on kernel ordering.
+  for (unsigned long expected : allowed) {
+    bool found = false;
+    for (unsigned i = 0; i != 3; ++i)
+      found |= out[i] == expected;
+    assert(found);
+  }
+
+  __msan_poison(out, sizeof(out));
+  assert(cap_ioctls_get(fd, out, 2) == 3);
+  assert(__msan_test_shadow(out, 2 * sizeof(*out)) == -1);
+  assert(__msan_test_shadow(out + 2, 3 * sizeof(*out)) == 0);
+  __msan_poison(out, sizeof(out));
+  assert(cap_ioctls_get(fd, out, 0) == 3);
+  assert(__msan_test_shadow(out, sizeof(out)) == 0);
+  assert(cap_ioctls_get(fd, nullptr, 0) == 3);
+  assert(cap_ioctls_get(-1, out, 5) == -1);
+  assert(errno == EBADF);
+  assert(__msan_test_shadow(out, sizeof(out)) == 0);
+
+  assert(cap_ioctls_limit(fd, nullptr, 0) == 0);
+  assert(cap_ioctls_get(fd, out, 5) == 0);
+  assert(__msan_test_shadow(out, sizeof(out)) == 0);
+  assert(close(fd) == 0);
+}
+
+int main(int argc, char **argv) {
+  if (argc == 1) {
+    test_rights();
+    test_outputs();
+    return 0;
+  }
+
+  assert(argc == 2);
+  cap_rights_t rights, other;
+  cap_rights_init(&rights, CAP_READ);
+  cap_rights_init(&other, CAP_WRITE);
+  __msan_poison(&rights, sizeof(rights));
+  if (!strcmp(argv[1], "set"))
+    cap_rights_set(&rights, CAP_WRITE);
+  else if (!strcmp(argv[1], "clear"))
+    cap_rights_clear(&rights, CAP_READ);
+  else if (!strcmp(argv[1], "merge"))
+    cap_rights_merge(&rights, &other);
+  else if (!strcmp(argv[1], "remove"))
+    cap_rights_remove(&rights, &other);
+  else if (!strcmp(argv[1], "limit"))
+    cap_rights_limit(STDIN_FILENO, &rights);
+  else
+    assert(false && "unknown negative control");
+  return 0;
+}
+
+// SET: Uninitialized bytes in __cap_rights_set
+// SET: WARNING: MemorySanitizer: use-of-uninitialized-value
+// CLEAR: Uninitialized bytes in __cap_rights_clear
+// CLEAR: WARNING: MemorySanitizer: use-of-uninitialized-value
+// MERGE: Uninitialized bytes in cap_rights_merge
+// MERGE: WARNING: MemorySanitizer: use-of-uninitialized-value
+// REMOVE: Uninitialized bytes in cap_rights_remove
+// REMOVE: WARNING: MemorySanitizer: use-of-uninitialized-value
+// LIMIT: Uninitialized bytes in cap_rights_limit
+// LIMIT: WARNING: MemorySanitizer: use-of-uninitialized-value
