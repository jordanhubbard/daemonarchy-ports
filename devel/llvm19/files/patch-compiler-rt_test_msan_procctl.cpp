--- /dev/null
+++ compiler-rt/test/msan/procctl.cpp
@@ -0,0 +1,56 @@
+// REQUIRES: target={{.*freebsd.*}}
+// RUN: %clangxx_msan -O0 %s -o %t
+// RUN: %run %t
+// RUN: not %run %t input 2>&1 | FileCheck %s
+
+#include <assert.h>
+#include <sys/procctl.h>
+#include <unistd.h>
+
+#include <sanitizer/msan_interface.h>
+
+int main(int argc, char **argv) {
+  if (argc > 1) {
+    int control;
+    // An invalid PID prevents changing any process control even if the
+    // interceptor fails to reject the uninitialized input.
+    return procctl(P_PID, static_cast<id_t>(-1), PROC_TRACE_CTL, &control);
+    // CHECK: WARNING: MemorySanitizer: use-of-uninitialized-value
+  }
+
+  procctl_reaper_status reaper;
+  int res = procctl(P_PID, getpid(), PROC_REAP_STATUS, &reaper);
+  assert(res == 0);
+  assert(__msan_test_shadow(&reaper, sizeof(reaper)) == -1);
+
+  const int commands[] = {
+      PROC_TRACE_STATUS, PROC_TRAPCAP_STATUS, PROC_PDEATHSIG_STATUS,
+      PROC_ASLR_STATUS,
+#ifdef PROC_PROTMAX_STATUS
+      PROC_PROTMAX_STATUS,
+#endif
+#ifdef PROC_STACKGAP_STATUS
+      PROC_STACKGAP_STATUS,
+#endif
+#ifdef PROC_NO_NEW_PRIVS_STATUS
+      PROC_NO_NEW_PRIVS_STATUS,
+#endif
+#ifdef PROC_WXMAP_STATUS
+      PROC_WXMAP_STATUS,
+#endif
+#ifdef PROC_LOGSIGEXIT_STATUS
+      PROC_LOGSIGEXIT_STATUS,
+#endif
+  };
+  for (int command : commands) {
+    int value;
+    res = procctl(P_PID, getpid(), command, &value);
+    assert(res == 0);
+    assert(__msan_test_shadow(&value, sizeof(value)) == -1);
+  }
+
+  int failed_output;
+  res = procctl(P_PID, static_cast<id_t>(-1), PROC_ASLR_STATUS, &failed_output);
+  assert(res == -1);
+  assert(__msan_test_shadow(&failed_output, sizeof(failed_output)) == 0);
+}
