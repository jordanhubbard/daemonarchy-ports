--- /dev/null
+++ clang/test/Driver/freebsd-shared-asan.c
@@ -0,0 +1,16 @@
+// RUN: %clang -### %s --target=i386-unknown-freebsd -fuse-ld=ld -fsanitize=address -shared-libsan -resource-dir=%S/Inputs/resource_dir --sysroot=%S/Inputs/basic_freebsd_tree 2>&1 | FileCheck %s --check-prefix=SHARED
+// RUN: %clang -### %s --target=i386-unknown-freebsd -fuse-ld=ld -fsanitize=address -shared-libsan -fno-sanitize-link-runtime -resource-dir=%S/Inputs/resource_dir --sysroot=%S/Inputs/basic_freebsd_tree 2>&1 | FileCheck %s --check-prefix=NO-RUNTIME
+// RUN: %clang -### %s --target=i386-unknown-freebsd -fuse-ld=ld -shared-libsan -resource-dir=%S/Inputs/resource_dir --sysroot=%S/Inputs/basic_freebsd_tree 2>&1 | FileCheck %s --check-prefix=NO-RUNTIME
+
+// A direct thread dependency precedes libc, avoiding its pthread stubs when
+// shared ASan initializes during process startup.
+// SHARED: "{{(.*[^-.0-9A-Z_a-z])?}}ld{{(.exe)?}}"
+// SHARED: libclang_rt.asan{{.*}}.so"
+// SHARED: "-lpthread"
+// SHARED: "-lc"
+
+// NO-RUNTIME: "{{(.*[^-.0-9A-Z_a-z])?}}ld{{(.exe)?}}"
+// NO-RUNTIME-NOT: libclang_rt.asan
+// NO-RUNTIME-NOT: "-lpthread"
+// NO-RUNTIME: "-lc"
+// NO-RUNTIME-NOT: "-lpthread"
