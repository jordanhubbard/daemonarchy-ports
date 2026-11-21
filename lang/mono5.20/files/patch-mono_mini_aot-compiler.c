--- mono/mini/aot-compiler.c.orig
+++ mono/mini/aot-compiler.c
@@ -11462,6 +11462,12 @@
 #define LD_OPTIONS "--shared"
 #elif defined(TARGET_POWERPC64)
 #define LD_OPTIONS "-m elf64ppc"
+#elif defined(TARGET_ARM64) && defined(__FreeBSD__)
+/*
+ * Keep the ADRP/ADD PLT layout decoded by mono_arch_patch_plt_entry ().
+ * LLD otherwise relaxes nearby entries to NOP/ADR.
+ */
+#define LD_OPTIONS "--no-relax"
 #endif
 
 #ifndef LD_OPTIONS
