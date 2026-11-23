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

The worker first switches to a separate observed SDK generation with all 52 replacement
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
The partial log contains269 FAIL and two XPASS result lines, including both
dynamic ASan discovery failures; these are incomplete observations. Missing
psutil also leaves coverage incomplete. The discovered count is not a
completed-case count.

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
including hashes of all 40 staged regular files. An exact independent copy of
the active SDK then receives the source-built psutil package through normal
package installation. The new observed SDK140 preserves every original139
prefix node and registry identity, and all140 package checksum/dependency
checks pass. Nobody reads all40 installed regular bodies and passes the
installed process, memory, thread and PID APIs. The preceding SDK139 prefix
and raw database remain unchanged; the persistent boot profile is unchanged.
Original psutil tests and independent disposable-VM/reboot acceptance stay open.

The complete dynamic-runtime source/work/stage/products boundary is separately
retained and independently verified on both physical filesystems (547,931
nodes). The complete SDK140 prefix/database, clone/install controls and local
installed-consumer evidence are also independently verified on both physical
filesystems (82,998 nodes). The next full unfiltered original `make test`
controller passes explicit SDK140/raw-database, complete source, ports, registry,
base, mount and stopped-data preflight guards. After28 changed-input preparatory
actions, LIT reports87562 entries and four workers. The run again ends with
Ninja137 and original make1 before a complete summary: the kernel records
memory-reclaim kills of a threaded-fork test process and the LIT driver. The
partial log contains43 FAIL and three XPASS result lines. All final SDK140/rawDB,
source, ports, registry, base, mount and stopped-data guards pass. These partial
entries are not an inventory of completed individual GoogleTest cases.
The complete failed-run boundary is now independently verified on both physical
filesystems: 942,954 nodes, including eight inert sockets, four FIFOs and all
regular bodies and hardlink groups. Before/after equality passes. Full suite
and fresh package acceptance remain open.

Read-only follow-up replays isolate two additional sanitizer problems.
Both original exit-statistics programs reach the memory-profile sysctl with
its output-buffer and length arguments misplaced. A separate native ordinary
user API probe reproduces the permission error and succeeds with the proper
argument order, returning the current process record and positive resident
pages. The original shared new/delete program also fails thread-key creation
with ENOSYS. Its trace reaches initialization through readlink from libc
startup. A debugger-only replay enables abort-on-error to obtain that trace;
a subsequent register-memory inspection fails at address zero, so the
debugger exit is1 and is not a test result. The separate plain replay retains
the original program options and exits1. The candidate corrects the FreeBSD
sysctl arguments. An initial early-readlink bypass was built but did not fix
the shared new/delete failure; it is removed from the revised candidate.

The full run also exposes FreeBSD driver and interpreter failures. Explicit
libstdc++ selection still produces libc++ linker arguments; its candidate
uses the existing ToolChain implementation for non-libc++ selection. Three
unchanged default interpreter exception replays crash with an out-of-range
Delta32 relocation. Three otherwise identical diagnostic invocations with
PIC enabled pass the original FileCheck assertions with ASLR still enabled.
The candidate makes PIC the FreeBSD interpreter default before user options.
The initial follow-up candidates and an additional FreeBSD TSan cleanup fix pass
fresh original checksum/patch targets without fuzz. Original native ports
`do-build` then successfully rebuilds Clang, the interpreter, static/dynamic
ASan and static TSan. Both unchanged exit-statistics scripts, the original
three-command stdlib driver script and the original default interpreter
exception script pass. Both default dynamic ASan unit programs still exit0.
The two original shared new/delete scripts continue to fail; this is partial
qualification. Every other warm source node, including historical backups and
caches, is preserved. All source, ports, SDK140/rawDB, registry, base, mount and
stopped-data guards pass.

The threaded-fork diagnosis isolates a FreeBSD TSan bookkeeping failure:
`thr_exit` takes an instrumentation-ignore fast path in the fork child without
marking the thread finished, so the parent allocates indefinitely while the
child waits in ThreadRegistry join. The runtime fix uses the raw interceptor
and tears down initialized live thread state even when instrumentation is
ignored. Public `thr_exit@FBSD_1.0` is the actual libthr import; private syscall
aliases are not the cause. ASan, MSan and UBSan original binaries return0 in
the paired guarded diagnosis. Its complete 37-node boundary is independently
verified on both physical filesystems before the warm build.

The rebuilt original TSan case now returns0 with unchanged compile arguments,
case body and `die_after_fork=0`. Earlier diagnostic stops at128MiB sampled RSS
and two seconds are retained failures, not passes: after the child finishes,
the original runtime deliberately sleeps during shutdown while the original
allocator thread continues. A subsequent standalone replay finishes naturally
within the10-second/8GiB diagnostic budget, reaching about2.7GiB sampled RSS.
No sanitizer option, kernel setting or ordinary testcase timeout changed.
This does not establish full original-target acceptance.

A separate shared-ASan paired control compiles the unchanged testcase twice.
Without pthread it fails its original FileCheck; adding only `-pthread` makes
those original checks pass, including the intentional heap-use-after-free
report. Both program exits are1 as that upstream test expects. ELF inspection
shows the latter has a direct libthr dependency before libc, avoiding libc's
pthread stubs during initialization. The revised FreeBSD driver requests
runtime dependencies for shared ASan when runtime linking is enabled. A new
driver regression also checks disabled runtime linking and shared-libsan with
no sanitizer enabled. The ineffective readlink bypass is removed. Fresh
original checksum/full patch passes without fuzz, and the new regression
detects the preceding compiler bug while its two negative controls pass.

The complete R12-R18 source/work/products and diagnostic boundary is
independently verified on both physical filesystems: 710,219 nodes, all bodies
and hardlinks, eight sockets, four FIFOs and the complete gzip stream.
Before/after equality passes. Only then does original ports do-build rebuild
the revised compiler and runtimes successfully. All source nodes except the
two explicit replacements and one new regression are preserved; SDK140/rawDB,
ports, registry, base, mounts and stopped-data guards pass.

All eleven subsequent targeted executions pass: both static/dynamic exit-stats
scripts, both original shared new/delete scripts, the stdlib driver script,
default interpreter exception, both default dynamic ASan programs (95 passes
and24 original disabled cases each), the new driver regression, the complete
original sanitizer driver regression, and the original TSan threaded-fork
compile/run script. Original assertions, options and runtime startup behavior
are retained. Compiler/runtime/script/unit inputs, case sources and mounts
remain unchanged; the owned jail is removed. Exact input bodies and compiled
case products are retained before the full target can overwrite output paths.

The R21 resource sampler used `ps -axo jid=,rss=`, which FreeBSD interprets as
one field and a replacement header. Its reported peak0 is invalid and its
intended RSS guard was ineffective; this is explicitly corrected in retained
evidence. The original script naturally exits0. Corrected three-field parsing
passes a separate native PID/jail/positive-RSS control. No R21 resource peak or
effective guard coverage is claimed; the earlier R18 measurement is separate.
The subsequent full unfiltered original target again ends incomplete with
original make1/Ninja137. The kernel kills huge_malloc and the LIT driver for
memory reclaim. The partial log contains38 FAIL and3 XPASS lines; no final
summary or requested JSON/xUnit reports are written. All final complete source,
SDK140/rawDB, ports, registry, base, mount and stopped-data guards pass. The
failed source/work/products and controls are being retained before warm mutation.
Original testcase assertions and selection remain unchanged; the new driver
regression is additive.

The next full run must retain upstream JSON and xUnit per-test reports as well
as complete output, inventories and final guards. LIT JSON omits user-skipped
entries, so xUnit is retained too. The original test environment whitelist
does not pass LIT_OPTS to test subprocesses. The R22 run added these reporting options, but LIT was killed before either
report was emitted. The older failed run retained its original configuration.


Separate API diagnosis leaves that frozen generation unchanged. The original
FreeBSD procctl assertion compares a signed value against the policy minimum,
but the [documented status](https://man.freebsd.org/cgi/man.cgi?query=procctl&sektion=2&manpath=FreeBSD+16.0-CURRENT)
also includes PROC_ASLR_ACTIVE. A native query returns raw0x80000003: valid
policy3 plus that flag, rejected by the original comparison. The candidate
masks only the local flag value and checks the exact legal policies. This is
an explicit portability assertion correction; it does not change ASLR policy.
Base/ASan/TSan/UBSan isolated candidates pass; MSan still reports uninitialized
input in its procctl interceptor, so the test correction alone is insufficient.

The original spawn test zero-initializes opaque attribute/action handles and
passes them without their initialization APIs. FreeBSD documents initialization
of [attributes](https://raw.githubusercontent.com/freebsd/freebsd-src/main/lib/libc/gen/posix_spawnattr_init.3)
and [file actions](https://man.freebsd.org/cgi/man.cgi?query=posix_spawn_file_actions_init&sektion=3).
The corrected source initializes and destroys both, preserves both spawn calls,
arguments and output checks, and adds waitpid/normal-child-exit assertions.
All side-effecting API calls remain outside assert. Both unchanged and corrected
sources are exercised with the base compiler and four original sanitizer flag
sets. Every original spawn case fails; all five corrected programs and their
original output checks pass. This strengthens the original test instead of
skipping it. No expectation annotations, filters or sanitizer suppressions change.

The additional runtime patch correctly models output-only reaper-status and
standard integer-status queries: do not read the output as input, and mark it
written only on success. Existing control-input and mixed reaper paths remain.
A new MSan regression detects the old false read while retaining the warning
for genuinely uninitialized control input. Failed queries must retain poison.
Fresh original checksum/patch passes without fuzz. In a separate complete cold
source tree, original make configure and the selected MSan/CXX-runtime/header
build both exit0 as nobody with all default component options and normalized
configuration preserved. This diagnostic uses one low-priority build job while
the then-running R22 suite kept its original four workers. Cold source, SDK140/rawDB,
ports, base, registry, mounts and stopped-data guards all pass.

Two diagnostic controller failures remain explicitly retained. An arbitrary
100KB archive-size minimum rejects the legitimate62KB CXX archive after the
successful build. A subsequent routing check incorrectly requires that C
source link the CXX archive. Independent archive-format/object-member and
native link/consumer checks replace those invalid criteria without rebuilding
or rewriting either failed result. The new status regression passes with
origin tracking off and on, failed-query poison checks pass, and uninitialized
input still gives the expected warning. Both corrected and unchanged original
common procctl MSan cases exit0 with the new runtime. The already compiled C
spawn consumer correctly links only the fresh core archive, exits0, and passes
its original output checks. All bound consumer inputs remain unchanged.

Complete cold source/work/objects, ports, fixtures, controls, logs, compiled
products and exact copied input bodies are independently verified on both
physical filesystems: 400,902 nodes, all bodies and literal links, and the
complete gzip stream. Before/after equality passes. The new API patches are
not integrated into the failed R22 source. Full tests
with these changes, rebuilt other sanitizer runtimes, fresh complete packaging,
installed consumers and lifecycle/release acceptance remain open.

These checks establish neither full upstream suite success nor a fresh full
package build with the changed recipe. Full case discovery, per-project outcomes and explicit
skip reasons remain open. Independent installed compiler consumers, final
CURRENT integration, complete toolchain provenance, canonical Component/SDK/
lifecycle admission and full 1.6 release acceptance remain open. Raw worker
evidence stays private.
