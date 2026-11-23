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

That execution completed 1,663 preparatory build actions, then failed before
case discovery: generated build-tree LIT imports `lit`, but the original port
rewrites source imports to the installed `lit19` namespace. All final source,
dependency, registry, base, mount and stopped-data guards passed. The complete
second failure source, work, stage and test products are retained and independently
verified on both physical filesystems (435,967 nodes).

The additional fix keeps source imports upstream-compatible and applies the
same existing namespace substitutions to installed copies. The `lit`/`llvm-lit`
hardlink is created after rewriting; sed leaves no backup files in the package.
Fresh original checksum and patch targets pass. Running the original LIT install
hook on a separate complete stage copy reproduces all 12,340 original stage
entries, including bytes, modes, owners, flags, symlinks and hardlink groups.
The original stage is unchanged, and both raw-source and namespaced staged
LIT version commands exit zero without installing the root LLVM package.
An initial probe's three symlink modes differed because its umask differed;
that failure is retained, and a fresh probe with the original producer umask
passes all unchanged assertions.

The full original test target with both fixes now reaches test discovery but
exits 1: an installed PyYAML file is root-owned mode 0600 and unreadable to
the unprivileged test user. Its original dependency stage has the same mode.
Discovery also records two dynamic ASan GoogleTest programs crashing with
SIGBUS in libc and an MLIR unit program aborting on duplicate option
registration. These failures remain unresolved; no cases are filtered. Final
source, dependency, registry, base, mount and stopped-data guards pass. Exactly
18 selected source LIT files match their fresh original patched replacements;
all other extracted source nodes remain unchanged.

The dependency permission cause is the qualification controller's inherited
umask 077. A native causal probe using the installed build and installer
methods reproduces mode 0600 under 077 and 0644 under 022 with identical
source bytes. A read-only audit finds 8,009 unreadable Python payload files
across 46 packages; all were added during the LLVM dependency build, and
none belongs to the earlier 87-package baseline.

A fresh original default PyYAML checksum/package/check-plist run with build
child umask 022 passes. Its staged files are readable, and both pure-Python
and libyaml consumers pass as nobody with the exact staged module path
asserted. The initial staged consumer was blocked by its private fixture
ancestor mode 0700. Its complete failed frontier was retained and verified
on both physical filesystems before adding only that ancestor's search bit;
the remaining fixture and sealed package are unchanged. The installed SDK
remains unchanged. This does not qualify the original pytest target, an
installed package consumer or recovery of the other affected packages.
The complete LLVM discovery-failure frontier is also independently verified
on both physical filesystems (621,320 nodes).

These checks establish neither upstream test success nor a fresh full package
build with the changed recipe. Full case discovery, per-project outcomes and explicit
skip reasons remain open. Independent installed compiler consumers, final
CURRENT integration, complete toolchain provenance, canonical Component/SDK/
lifecycle admission and full 1.6 release acceptance remain open. Raw worker
evidence stays private.
