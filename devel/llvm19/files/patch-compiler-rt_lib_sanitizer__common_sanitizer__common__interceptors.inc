--- compiler-rt/lib/sanitizer_common/sanitizer_common_interceptors.inc.orig
+++ compiler-rt/lib/sanitizer_common/sanitizer_common_interceptors.inc
@@ -8388,72 +8388,76 @@
 #endif
 
 #if SANITIZER_INTERCEPT_CAPSICUM
-#define CAP_RIGHTS_INIT_INTERCEPTOR(cap_rights_init, rights, ...)          \
-  {                                                                        \
-    void *ctx;                                                             \
-    COMMON_INTERCEPTOR_ENTER(ctx, cap_rights_init, rights, ##__VA_ARGS__); \
-    if (rights)                                                            \
-      COMMON_INTERCEPTOR_READ_RANGE(ctx, rights, sizeof(*rights));         \
-    __sanitizer_cap_rights_t *ret =                                        \
-        REAL(cap_rights_init)(rights, ##__VA_ARGS__);                      \
-    if (ret)                                                               \
-      COMMON_INTERCEPTOR_WRITE_RANGE(ctx, ret, sizeof(*ret));              \
-    return ret;                                                            \
-  }
-
-#define CAP_RIGHTS_SET_INTERCEPTOR(cap_rights_set, rights, ...)           \
-  {                                                                       \
-    void *ctx;                                                            \
-    COMMON_INTERCEPTOR_ENTER(ctx, cap_rights_set, rights, ##__VA_ARGS__); \
-    if (rights)                                                           \
-      COMMON_INTERCEPTOR_READ_RANGE(ctx, rights, sizeof(*rights));        \
-    __sanitizer_cap_rights_t *ret =                                       \
-        REAL(cap_rights_set)(rights, ##__VA_ARGS__);                      \
-    if (ret)                                                              \
-      COMMON_INTERCEPTOR_WRITE_RANGE(ctx, ret, sizeof(*ret));             \
-    return ret;                                                           \
-  }
-
-#define CAP_RIGHTS_CLEAR_INTERCEPTOR(cap_rights_clear, rights, ...)         \
-  {                                                                         \
-    void *ctx;                                                              \
-    COMMON_INTERCEPTOR_ENTER(ctx, cap_rights_clear, rights, ##__VA_ARGS__); \
-    if (rights)                                                             \
-      COMMON_INTERCEPTOR_READ_RANGE(ctx, rights, sizeof(*rights));          \
-    __sanitizer_cap_rights_t *ret =                                         \
-        REAL(cap_rights_clear)(rights, ##__VA_ARGS__);                      \
-    if (ret)                                                                \
-      COMMON_INTERCEPTOR_WRITE_RANGE(ctx, ret, sizeof(*ret));               \
-    return ret;                                                             \
-  }
-
-#define CAP_RIGHTS_IS_SET_INTERCEPTOR(cap_rights_is_set, rights, ...)        \
-  {                                                                          \
-    void *ctx;                                                               \
-    COMMON_INTERCEPTOR_ENTER(ctx, cap_rights_is_set, rights, ##__VA_ARGS__); \
-    if (rights)                                                              \
-      COMMON_INTERCEPTOR_READ_RANGE(ctx, rights, sizeof(*rights));           \
-    return REAL(cap_rights_is_set)(rights, ##__VA_ARGS__);                   \
-  }
-
-INTERCEPTOR(__sanitizer_cap_rights_t *, cap_rights_init,
-            __sanitizer_cap_rights_t *rights) {
-  CAP_RIGHTS_INIT_INTERCEPTOR(cap_rights_init, rights);
-}
-
-INTERCEPTOR(__sanitizer_cap_rights_t *, cap_rights_set,
-            __sanitizer_cap_rights_t *rights) {
-  CAP_RIGHTS_SET_INTERCEPTOR(cap_rights_set, rights);
-}
-
-INTERCEPTOR(__sanitizer_cap_rights_t *, cap_rights_clear,
-            __sanitizer_cap_rights_t *rights) {
-  CAP_RIGHTS_CLEAR_INTERCEPTOR(cap_rights_clear, rights);
-}
-
-INTERCEPTOR(bool, cap_rights_is_set,
-            __sanitizer_cap_rights_t *rights) {
-  CAP_RIGHTS_IS_SET_INTERCEPTOR(cap_rights_is_set, rights);
+// The public cap_rights_* macros call the versioned/variadic __cap_rights_*
+// interfaces. Forward one right at a time to libc, which retains validation
+// and the native representation, without imposing a limit on the argument list.
+INTERCEPTOR(__sanitizer_cap_rights_t *, __cap_rights_set,
+            __sanitizer_cap_rights_t *rights, ...) {
+  void *ctx;
+  COMMON_INTERCEPTOR_ENTER(ctx, __cap_rights_set, rights);
+  if (rights)
+    COMMON_INTERCEPTOR_READ_RANGE(ctx, rights, sizeof(*rights));
+  __sanitizer_cap_rights_t *ret = REAL(__cap_rights_set)(rights, 0ULL);
+  va_list ap;
+  va_start(ap, rights);
+  while (unsigned long long right = va_arg(ap, unsigned long long))
+    ret = REAL(__cap_rights_set)(rights, right, 0ULL);
+  va_end(ap);
+  if (ret)
+    COMMON_INTERCEPTOR_WRITE_RANGE(ctx, ret, sizeof(*ret));
+  return ret;
+}
+
+INTERCEPTOR(__sanitizer_cap_rights_t *, __cap_rights_init, int version,
+            __sanitizer_cap_rights_t *rights, ...) {
+  void *ctx;
+  COMMON_INTERCEPTOR_ENTER(ctx, __cap_rights_init, version, rights);
+  // Initialization writes rights; it must not require initialized input bytes.
+  __sanitizer_cap_rights_t *ret = REAL(__cap_rights_init)(version, rights, 0ULL);
+  va_list ap;
+  va_start(ap, rights);
+  while (unsigned long long right = va_arg(ap, unsigned long long))
+    ret = REAL(__cap_rights_set)(ret, right, 0ULL);
+  va_end(ap);
+  if (ret)
+    COMMON_INTERCEPTOR_WRITE_RANGE(ctx, ret, sizeof(*ret));
+  return ret;
+}
+
+INTERCEPTOR(__sanitizer_cap_rights_t *, __cap_rights_clear,
+            __sanitizer_cap_rights_t *rights, ...) {
+  void *ctx;
+  COMMON_INTERCEPTOR_ENTER(ctx, __cap_rights_clear, rights);
+  if (rights)
+    COMMON_INTERCEPTOR_READ_RANGE(ctx, rights, sizeof(*rights));
+  __sanitizer_cap_rights_t *ret = REAL(__cap_rights_clear)(rights, 0ULL);
+  va_list ap;
+  va_start(ap, rights);
+  while (unsigned long long right = va_arg(ap, unsigned long long))
+    ret = REAL(__cap_rights_clear)(rights, right, 0ULL);
+  va_end(ap);
+  if (ret)
+    COMMON_INTERCEPTOR_WRITE_RANGE(ctx, ret, sizeof(*ret));
+  return ret;
+}
+
+INTERCEPTOR(bool, __cap_rights_is_set,
+            const __sanitizer_cap_rights_t *rights, ...) {
+  void *ctx;
+  COMMON_INTERCEPTOR_ENTER(ctx, __cap_rights_is_set, rights);
+  if (rights)
+    COMMON_INTERCEPTOR_READ_RANGE(ctx, rights, sizeof(*rights));
+  bool ret = REAL(__cap_rights_is_set)(rights, 0ULL);
+  va_list ap;
+  va_start(ap, rights);
+  while (ret) {
+    unsigned long long right = va_arg(ap, unsigned long long);
+    if (!right)
+      break;
+    ret = REAL(__cap_rights_is_set)(rights, right, 0ULL);
+  }
+  va_end(ap);
+  return ret;
 }
 
 INTERCEPTOR(int, cap_rights_limit, int fd,
@@ -8466,10 +8470,11 @@
   return REAL(cap_rights_limit)(fd, rights);
 }
 
-INTERCEPTOR(int, cap_rights_get, int fd, __sanitizer_cap_rights_t *rights) {
-  void *ctx;
-  COMMON_INTERCEPTOR_ENTER(ctx, cap_rights_get, fd, rights);
-  int ret = REAL(cap_rights_get)(fd, rights);
+INTERCEPTOR(int, __cap_rights_get, int version, int fd,
+            __sanitizer_cap_rights_t *rights) {
+  void *ctx;
+  COMMON_INTERCEPTOR_ENTER(ctx, __cap_rights_get, version, fd, rights);
+  int ret = REAL(__cap_rights_get)(version, fd, rights);
   if (!ret && rights)
     COMMON_INTERCEPTOR_WRITE_RANGE(ctx, rights, sizeof(*rights));
 
@@ -8489,6 +8494,8 @@
   __sanitizer_cap_rights *dst, const __sanitizer_cap_rights *src) {
   void *ctx;
   COMMON_INTERCEPTOR_ENTER(ctx, cap_rights_merge, dst, src);
+  if (dst)
+    COMMON_INTERCEPTOR_READ_RANGE(ctx, dst, sizeof(*dst));
   if (src)
     COMMON_INTERCEPTOR_READ_RANGE(ctx, src, sizeof(*src));
 
@@ -8503,6 +8510,8 @@
   __sanitizer_cap_rights *dst, const __sanitizer_cap_rights *src) {
   void *ctx;
   COMMON_INTERCEPTOR_ENTER(ctx, cap_rights_remove, dst, src);
+  if (dst)
+    COMMON_INTERCEPTOR_READ_RANGE(ctx, dst, sizeof(*dst));
   if (src)
     COMMON_INTERCEPTOR_READ_RANGE(ctx, src, sizeof(*src));
 
@@ -8534,21 +8543,24 @@
   return REAL(cap_ioctls_limit)(fd, cmds, ncmds);
 }
 
-INTERCEPTOR(int, cap_ioctls_get, int fd, uptr *cmds, SIZE_T maxcmds) {
+INTERCEPTOR(SSIZE_T, cap_ioctls_get, int fd, uptr *cmds, SIZE_T maxcmds) {
   void *ctx;
   COMMON_INTERCEPTOR_ENTER(ctx, cap_ioctls_get, fd, cmds, maxcmds);
-  int ret = REAL(cap_ioctls_get)(fd, cmds, maxcmds);
-  if (!ret && cmds)
-    COMMON_INTERCEPTOR_WRITE_RANGE(ctx, cmds, sizeof(*cmds) * maxcmds);
+  SSIZE_T ret = REAL(cap_ioctls_get)(fd, cmds, maxcmds);
+  // CAP_IOCTLS_ALL reports unrestricted access without writing the array.
+  // Otherwise the return is the total count, possibly larger than maxcmds.
+  if (ret >= 0 && static_cast<uptr>(ret) != cap_ioctls_all && cmds)
+    COMMON_INTERCEPTOR_WRITE_RANGE(ctx, cmds,
+                                   sizeof(*cmds) * Min((SIZE_T)ret, maxcmds));
 
   return ret;
 }
 #define INIT_CAPSICUM                          \
-  COMMON_INTERCEPT_FUNCTION(cap_rights_init); \
-  COMMON_INTERCEPT_FUNCTION(cap_rights_set); \
-  COMMON_INTERCEPT_FUNCTION(cap_rights_clear); \
-  COMMON_INTERCEPT_FUNCTION(cap_rights_is_set); \
-  COMMON_INTERCEPT_FUNCTION(cap_rights_get);   \
+  COMMON_INTERCEPT_FUNCTION(__cap_rights_set); \
+  COMMON_INTERCEPT_FUNCTION(__cap_rights_init); \
+  COMMON_INTERCEPT_FUNCTION(__cap_rights_clear); \
+  COMMON_INTERCEPT_FUNCTION(__cap_rights_is_set); \
+  COMMON_INTERCEPT_FUNCTION(__cap_rights_get);   \
   COMMON_INTERCEPT_FUNCTION(cap_rights_limit); \
   COMMON_INTERCEPT_FUNCTION(cap_rights_contains); \
   COMMON_INTERCEPT_FUNCTION(cap_rights_remove); \
@@ -9982,34 +9994,57 @@
 
 #if SANITIZER_INTERCEPT_PROCCTL
 INTERCEPTOR(int, procctl, int idtype, u64 id, int cmd, uptr data) {
-   void *ctx;
-   COMMON_INTERCEPTOR_ENTER(ctx, procctl, idtype, id, cmd, data);
-   static const int PROC_REAP_ACQUIRE = 2;
-   static const int PROC_REAP_RELEASE = 3;
-   static const int PROC_REAP_STATUS = 4;
-   static const int PROC_REAP_GETPIDS = 5;
-   static const int PROC_REAP_KILL = 6;
-   if (cmd < PROC_REAP_ACQUIRE || cmd > PROC_REAP_KILL) {
-     COMMON_INTERCEPTOR_READ_RANGE(ctx, (void *)data, sizeof(int));
-   } else {
-     // reap_acquire/reap_release bears no arguments.
-     if (cmd > PROC_REAP_RELEASE) {
-       unsigned int reapsz;
-       switch (cmd) {
-       case PROC_REAP_STATUS:
-         reapsz = struct_procctl_reaper_status_sz;
-         break;
-       case PROC_REAP_GETPIDS:
-         reapsz = struct_procctl_reaper_pids_sz;
-         break;
-       case PROC_REAP_KILL:
-         reapsz = struct_procctl_reaper_kill_sz;
-         break;
-       }
-       COMMON_INTERCEPTOR_READ_RANGE(ctx, (void *)data, reapsz);
-     }
-   }
-   return REAL(procctl)(idtype, id, cmd, data);
+  void *ctx;
+  COMMON_INTERCEPTOR_ENTER(ctx, procctl, idtype, id, cmd, data);
+  static const int PROC_REAP_ACQUIRE = 2;
+  static const int PROC_REAP_RELEASE = 3;
+  static const int PROC_REAP_STATUS = 4;
+  static const int PROC_REAP_GETPIDS = 5;
+  static const int PROC_REAP_KILL = 6;
+  static const int PROC_TRACE_STATUS = 8;
+  static const int PROC_TRAPCAP_STATUS = 10;
+  static const int PROC_PDEATHSIG_STATUS = 12;
+  static const int PROC_ASLR_STATUS = 14;
+  static const int PROC_PROTMAX_STATUS = 16;
+  static const int PROC_STACKGAP_STATUS = 18;
+  static const int PROC_NO_NEW_PRIVS_STATUS = 20;
+  static const int PROC_WXMAP_STATUS = 22;
+  static const int PROC_LOGSIGEXIT_STATUS = 24;
+  uptr read_sz = sizeof(int), write_sz = 0;
+  switch (cmd) {
+  case PROC_REAP_ACQUIRE:
+  case PROC_REAP_RELEASE:
+    read_sz = 0;
+    break;
+  case PROC_REAP_STATUS:
+    read_sz = 0;
+    write_sz = struct_procctl_reaper_status_sz;
+    break;
+  case PROC_REAP_GETPIDS:
+    read_sz = struct_procctl_reaper_pids_sz;
+    break;
+  case PROC_REAP_KILL:
+    read_sz = struct_procctl_reaper_kill_sz;
+    break;
+  case PROC_TRACE_STATUS:
+  case PROC_TRAPCAP_STATUS:
+  case PROC_PDEATHSIG_STATUS:
+  case PROC_ASLR_STATUS:
+  case PROC_PROTMAX_STATUS:
+  case PROC_STACKGAP_STATUS:
+  case PROC_NO_NEW_PRIVS_STATUS:
+  case PROC_WXMAP_STATUS:
+  case PROC_LOGSIGEXIT_STATUS:
+    read_sz = 0;
+    write_sz = sizeof(int);
+    break;
+  }
+  if (read_sz)
+    COMMON_INTERCEPTOR_READ_RANGE(ctx, (void *)data, read_sz);
+  int res = REAL(procctl)(idtype, id, cmd, data);
+  if (res == 0 && write_sz)
+    COMMON_INTERCEPTOR_WRITE_RANGE(ctx, (void *)data, write_sz);
+  return res;
 }
 #define INIT_PROCCTL COMMON_INTERCEPT_FUNCTION(procctl)
 #else
