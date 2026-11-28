--- compiler-rt/test/msan/dtls_test.c.orig
+++ compiler-rt/test/msan/dtls_test.c
@@ -6,7 +6,7 @@
    see https://sourceware.org/bugzilla/show_bug.cgi?id=16291
    and https://github.com/google/sanitizers/issues/547
 
-   XFAIL: target={{.*freebsd.*}}
+   XFAIL: target={{.*freebsd.*}} && !target={{x86_64-.*}}
    UNSUPPORTED: target=powerpc{{.*}}
 
    // Reports use-of-uninitialized-value, not analyzed
