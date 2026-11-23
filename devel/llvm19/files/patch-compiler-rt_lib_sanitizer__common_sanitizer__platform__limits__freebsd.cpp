--- compiler-rt/lib/sanitizer_common/sanitizer_platform_limits_freebsd.cpp.orig
+++ compiler-rt/lib/sanitizer_common/sanitizer_platform_limits_freebsd.cpp
@@ -100,12 +100,16 @@
 
 namespace __sanitizer {
 void *__sanitizer_get_link_map_by_dlopen_handle(void *handle) {
+  // A failed dlopen has no mapping. dlinfo(nullptr) queries the caller instead.
+  if (!handle)
+    return nullptr;
   void *p = nullptr;
   return internal_dlinfo(handle, RTLD_DI_LINKMAP, &p) == 0 ? p : nullptr;
 }
 
 unsigned struct_cpuset_sz = sizeof(cpuset_t);
 unsigned struct_cap_rights_sz = sizeof(cap_rights_t);
+uptr cap_ioctls_all = CAP_IOCTLS_ALL;
 unsigned struct_utsname_sz = sizeof(struct utsname);
 unsigned struct_stat_sz = sizeof(struct stat);
 unsigned struct_rusage_sz = sizeof(struct rusage);
