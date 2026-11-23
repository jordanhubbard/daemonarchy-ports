--- compiler-rt/lib/sanitizer_common/sanitizer_procmaps_bsd.cpp.orig
+++ compiler-rt/lib/sanitizer_common/sanitizer_procmaps_bsd.cpp
@@ -36,9 +36,9 @@
   uptr Len = sizeof(*InfoProc);
   uptr Size = Len;
   InfoProc = (struct kinfo_proc *)MmapOrDie(Size, "GetMemoryProfile()");
   CHECK_EQ(
-      internal_sysctl(Mib, ARRAY_SIZE(Mib), nullptr, (uptr *)InfoProc, &Len, 0),
+      internal_sysctl(Mib, ARRAY_SIZE(Mib), InfoProc, &Len, nullptr, 0),
       0);
   cb(0, InfoProc->ki_rssize * GetPageSizeCached(), false, stats);
   UnmapOrDie(InfoProc, Size, true);
 }
