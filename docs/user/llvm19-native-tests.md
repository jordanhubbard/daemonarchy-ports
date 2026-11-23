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
registration. At that checkpoint these failures remained unresolved; no cases were filtered. Final
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
the remaining fixture and sealed package are unchanged. At that staged check, the installed SDK
remained unchanged. This does not qualify the original pytest target, an
installed package consumer or recovery of the other affected packages.
The complete LLVM discovery-failure frontier is also independently verified
on both physical filesystems (621,320 nodes).

The remaining 45 affected Python packages also pass fresh original checksum,
package and plist targets with the original versions, options, distinfo and
dependency declarations. Nobody reads and verifies the hashes of all 9,364
regular staged files. Their complete source, work, stage, packages, inputs and
logs are retained and independently verified on both physical filesystems
(51,332 nodes). Together with PyYAML, all 46 replacement packages are built;
the installed SDK was unchanged during these source rebuilds. Their original
unit tests and sealed installed consumers remained pending at that point.

The six non-Python dependencies from the same build each also have a generated
SBOM file with mode 0600, although their runtime files are readable. Fresh
original builds of ca_root_nss, libedit, Lua 5.3, Lua 5.4, snowballstemmer and
SWIG pass checksum/package/plist and nobody verifies all 6,122 staged file
bodies. Their complete frontier is independently verified on both physical
filesystems (17,267 nodes). All 52 replacement dependency packages are now
built; their original tests and installed acceptance were still pending at
that staged-build boundary.

The MLIR discovery crash comes from the unit target explicitly linking static
native-codegen libraries alongside shared LLVM. The added CMake patch declares
`LLVM_LINK_COMPONENTS` before creating the unit target, allowing the existing
LLVM helper to honor shared-library selection. Fresh original checksum and
patch targets pass without fuzz. The original configure target runs with a
separate new configure cookie; no old completion cookie is deleted. The
original `make test` entrypoint then builds `MLIRTargetLLVMTests` as nobody
using an explicit diagnostic target override. Running that binary without case
filters discovers all seven original cases: five pass, and the original cases
skip PTXAS binary generation and ROCm binary generation because those tools
are absent. All C++ cases and assertions are unchanged. Final SDK, registry,
base, mount, stopped-data and source guards pass, with only the explicit CMake
file changed relative to the retained preceding source frontier. The complete
post-change source, work, stage, test products and controls are independently
verified on both physical filesystems (596,640 nodes). Full LLVM
`check-all`, fresh package acceptance and the sanitizer failures remain open.

The worker now runs a separate observed SDK generation with all 52 replacement
packages installed through normal package operations and original scripts.
Before switching the running worker, the original prefix/database and an
independent copy matched all 82,744 entries, with disjoint regular-file inodes.
The complete pre-transition boundary, including certificate state and loader
hints, is independently verified on both physical filesystems (165,677 nodes).
The original prefix and raw database remain unchanged for rollback.

All 139 installed package checksums and no-fetch dependency checks pass, all
registry names/versions/origins remain the same, and 60,473 paths owned by the
original 87 packages are unchanged. Nobody reads and hash-verifies all 15,554
installed regular files belonging to the replacement packages. YAML/libyaml
parsing, Jinja2 rendering, Docutils HTML rendering, dependency imports and the
Sphinx CLI also pass as nobody. The complete new SDK/database, control evidence,
consumer fixtures, certificates and loader hints are independently retained
and verified on both physical filesystems (83,051 nodes).

This is local installed-consumer evidence. Original dependency unit tests,
independent disposable-VM acceptance and reboot qualification remain open.
The persistent boot profile still selects the retained original SDK; the
new generation is selected by the running worker's explicit mount aliases.
The subsequent full unfiltered original `make test` run discovers 87,540
tests but ends before a complete summary: original make exits 1 and Ninja
reports 137. The kernel records memory-reclaim kills of two test programs and
the Python LIT driver. Four selected Ninja build jobs did not constrain LIT,
which used eight workers. A subsequent run must bind actual LIT concurrency
to the worker resource budget while retaining every original case. All final
active-SDK, source, ports, registry, base, stopped-data and mount guards pass.
Missing psutil and two dynamic ASan programs failing discovery also leave
coverage incomplete; the discovered count is not a completed-case count.

Native debugger observations identify a separate ASan startup mechanism:
its TLS beginning is unsigned -128, yielding an invalid shadow address.
Original static TLS discovery assumes module ID 1, while a plain native
`dl_iterate_phdr` probe observes libc TLS at module ID 2 and no module ID 1,
both outside and inside the isolated jail. The absent anchor produces zero
before the FreeBSD 128-byte TLS reserve subtraction. A local patch candidate
selects the minimum allocated FreeBSD TLS module ID. Fresh original checksum
and patch targets pass without fuzz, and the original ports `do-build` target
rebuilds the dynamic ASan runtime as nobody. Both unchanged original dynamic
unit programs now discover 119 cases each and pass all 95 default enabled
cases each; 24 cases per program remain upstream-disabled. No test assertions,
case bodies, filters or ASAN_OPTIONS are changed. All final source (except the
explicit TLS implementation file), ports, SDK, raw database, registry, base,
mount and stopped-data guards pass. Static sanitizer runtimes, broader
sanitizer tests and full LLVM qualification remain pending.

The complete failed-run source, work, test products, controls and all four
native debugger diagnoses are independently retained and verified on both
physical filesystems: 547,967 nodes, including four inert Unix sockets and
two FIFOs. Standard newc headers preserve their types; FreeBSD bsdtar needs
the retained manifest-bound helper to recreate sockets after extraction.
An actual native socket/FIFO replay passes, as do complete body, hardlink
and gzip-stream checks. The original native files remain retained.

The revised port candidate declares psutil as a test dependency and passes
the selected make job count through `LIT_OPTS`, whose upstream argument
parser gives the environment precedence over generated command arguments.
The runtime change increments the port revision to 6. Native metadata checks
confirm the selected psutil flavor and a LIT worker limit of four. Psutil also passes
fresh original checksum/package/plist targets and ordinary-user staged APIs,
including hashes of all 40 staged regular files; installed acceptance and its
original upstream tests remain open. Full LLVM and fresh package acceptance
remain pending.

These checks establish neither full upstream suite success nor a fresh full
package build with the changed recipe. Full case discovery, per-project outcomes and explicit
skip reasons remain open. Independent installed compiler consumers, final
CURRENT integration, complete toolchain provenance, canonical Component/SDK/
lifecycle admission and full 1.6 release acceptance remain open. Raw worker
evidence stays private.
