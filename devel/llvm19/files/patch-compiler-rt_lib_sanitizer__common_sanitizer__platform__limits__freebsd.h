--- compiler-rt/lib/sanitizer_common/sanitizer_platform_limits_freebsd.h.orig
+++ compiler-rt/lib/sanitizer_common/sanitizer_platform_limits_freebsd.h
@@ -730,6 +730,7 @@
 
 typedef struct __sanitizer_cap_rights __sanitizer_cap_rights_t;
 extern unsigned struct_cap_rights_sz;
+extern uptr cap_ioctls_all;
 
 extern unsigned struct_fstab_sz;
 extern unsigned struct_StringList_sz;
