--- compiler-rt/lib/sanitizer_common/sanitizer_linux_libcdep.cpp.orig
+++ compiler-rt/lib/sanitizer_common/sanitizer_linux_libcdep.cpp
@@ -53,7 +53,7 @@
 // that, it was never implemented. So just define it to zero.
 #    undef MAP_NORESERVE
 #    define MAP_NORESERVE 0
-extern const Elf_Auxinfo *__elf_aux_vector;
+extern const Elf_Auxinfo *__elf_aux_vector __attribute__ ((weak));
 extern "C" int __sys_sigaction(int signum, const struct sigaction *act,
                                struct sigaction *oldact);
 #  endif
@@ -440,11 +440,20 @@
   dl_iterate_phdr(CollectStaticTlsBlocks, &ranges);
   uptr len = ranges.size();
   Sort(ranges.begin(), len);
-  // Find the range with tls_modid == main_tls_modid. For glibc, because
-  // libc.so uses PT_TLS, this module is guaranteed to exist and is one of
-  // the initially loaded modules.
+  size_t anchor_tls_modid = main_tls_modid;
+#    if SANITIZER_FREEBSD
+  // FreeBSD's first TLS-bearing module need not have module ID 1.
+  // Use the smallest allocated ID to anchor the initial static TLS block.
+  if (len != 0) {
+    anchor_tls_modid = ranges[0].tls_modid;
+    for (uptr i = 1; i != len; ++i)
+      anchor_tls_modid = Min(anchor_tls_modid, ranges[i].tls_modid);
+  }
+#    endif
+  // On glibc, libc.so guarantees that main_tls_modid is initially loaded
+  // and uses PT_TLS.
   uptr one = 0;
-  while (one != len && ranges[one].tls_modid != main_tls_modid) ++one;
+  while (one != len && ranges[one].tls_modid != anchor_tls_modid) ++one;
   if (one == len) {
     // This may happen with musl if no module uses PT_TLS.
     *addr = 0;
