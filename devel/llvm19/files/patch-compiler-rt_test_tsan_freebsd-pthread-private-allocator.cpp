--- /dev/null
+++ compiler-rt/test/tsan/freebsd-pthread-private-allocator.cpp
@@ -0,0 +1,86 @@
+// RUN: %clangxx_tsan -O1 -fno-builtin-memset %s -o %t
+// RUN: %run %t 2>&1 | FileCheck %s --check-prefix=CLEAN
+// RUN: %deflake %run %t race | FileCheck %s --check-prefix=RACE
+// REQUIRES: target={{.*freebsd.*}}
+// REQUIRES: stable-runtime
+
+#include <assert.h>
+#include <errno.h>
+#include <pthread.h>
+#include <stdint.h>
+#include <stdio.h>
+#include <string.h>
+
+static pthread_key_t key;
+static pthread_barrier_t barrier;
+static char shared[32];
+
+static void CheckPrivateAllocation() {
+  void *value = reinterpret_cast<void *>(uintptr_t(1));
+  int res = pthread_setspecific(key, value);
+  assert(res == 0);
+  assert(pthread_getspecific(key) == value);
+  res = pthread_setspecific(pthread_key_t(-1), value);
+  assert(res == EINVAL);
+  assert(pthread_getspecific(key) == value);
+  res = pthread_setspecific(key, nullptr);
+  assert(res == 0);
+
+  pthread_mutex_t mutex;
+  res = pthread_mutex_init(&mutex, nullptr);
+  assert(res == 0);
+  res = pthread_mutex_lock(&mutex);
+  assert(res == 0);
+  res = pthread_mutex_unlock(&mutex);
+  assert(res == 0);
+  res = pthread_mutex_destroy(&mutex);
+  assert(res == 0);
+}
+
+static void *Allocate(void *) {
+  CheckPrivateAllocation();
+  return nullptr;
+}
+
+static void *WriteShared(void *) {
+  CheckPrivateAllocation();
+  int res = pthread_barrier_wait(&barrier);
+  assert(res == 0 || res == PTHREAD_BARRIER_SERIAL_THREAD);
+  // This application access after the native calls must remain instrumented.
+  memset(shared, 1, sizeof(shared));
+  return nullptr;
+}
+
+int main(int argc, char **) {
+  int res = pthread_key_create(&key, nullptr);
+  assert(res == 0);
+  if (argc > 1) {
+    res = pthread_barrier_init(&barrier, nullptr, 2);
+    assert(res == 0);
+  }
+  for (int i = 0; i < (argc > 1 ? 1 : 64); ++i) {
+    pthread_t a, b;
+    res = pthread_create(&a, nullptr, argc > 1 ? WriteShared : Allocate, nullptr);
+    assert(res == 0);
+    res = pthread_create(&b, nullptr, argc > 1 ? WriteShared : Allocate, nullptr);
+    assert(res == 0);
+    res = pthread_join(a, nullptr);
+    assert(res == 0);
+    res = pthread_join(b, nullptr);
+    assert(res == 0);
+  }
+  if (argc > 1) {
+    res = pthread_barrier_destroy(&barrier);
+    assert(res == 0);
+  }
+  res = pthread_key_delete(key);
+  assert(res == 0);
+  fprintf(stderr, "DONE\n");
+}
+
+// CLEAN-NOT: WARNING: ThreadSanitizer
+// CLEAN: DONE
+// CLEAN-NOT: WARNING: ThreadSanitizer
+// RACE: WARNING: ThreadSanitizer: data race
+// RACE: memset
+// RACE: WriteShared
