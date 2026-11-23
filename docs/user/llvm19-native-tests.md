# LLVM 19 native upstream tests

The original `devel/llvm19` test recipe references `${WRKSRC}/test`,
`${WRKSRC}/Release/lib` and `check-local-lit`. With the current monorepo
release and out-of-source CMake build, native `make test` exits at the
ownership change because that source directory does not exist. No upstream
test cases run in that attempt.

The candidate uses the framework's configured `TEST_WRKSRC`, the upstream
`check-all` target and the selected Ninja build jobs. It preserves the
original unprivileged execution when invoked by root. Test HOME, temporary
files and XDG state live in writable directories inside the binary tree;
recursive ownership changes do not follow symlinks into source or installed
dependencies. Ordinary unprivileged builds continue to run as their caller.

The [initial native record](../../verification/llvm19-native-tests-20261123.json)
binds the original failure and first corrected-target execution. The complete
default package had already passed native checksum, package and plist checks,
with AMDGPU, WASM, Clang, compiler-rt, documentation, extras, LIT, LLD, LLDB,
MLIR, OpenMP, Polly, Python bindings and static libraries retained. The root
LLVM package is not installed in the worker's observed dependency environment.

Before the original test attempt, the complete source, work, stage, packages,
139-package observed dependency environment, HOME, temporary files, caches,
inputs and logs were retained. Both physical filesystems independently verify
all 585,313 archive nodes, all regular-file bodies, hardlink groups and the
complete gzip stream. After the failed attempt, every baseline node still
matches; all ten added nodes are retained in a separately verified additive
archive. Both archives are needed to reconstruct that failed frontier.

The corrected target uses a copied complete frozen ports view with only the
LLVM Makefile overlaid and the same previously built default variant. Native
metadata confirms identical configure arguments, source directory, options
and dependency declarations. Initial process observations show Ninja and
Clang running as UID 65534 while building upstream test programs. The private
worker's four root-owned work ancestors were explicitly made searchable for
that test user; their original permissions are recorded separately.

This is an initial execution observation, not a completed test-suite result.
Full case discovery, per-project outcomes, explicit skip reasons and final
source/dependency/data guards remain to be reviewed. Independent installed
compiler consumers, final CURRENT integration, complete toolchain provenance,
canonical Component/SDK/lifecycle admission and full 1.6 release acceptance
remain open. Raw worker evidence stays private.
