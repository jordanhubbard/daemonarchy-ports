--- compiler-rt/lib/sanitizer_common/sanitizer_common_interceptors.inc.orig
+++ compiler-rt/lib/sanitizer_common/sanitizer_common_interceptors.inc
@@ -9981,36 +9981,59 @@
 #endif

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
 #define INIT_PROCCTL
