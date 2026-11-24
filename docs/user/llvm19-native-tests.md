# LLVM 19 native upstream tests

## Current candidate and qualification boundary

The review candidate is `llvm19-19.1.7_7`. Production fixes change package
bytes, but no new package is published. R355 normal configure/full build and
R364 actual normal runtime checks pass for the twelve libc++ additions plus the
short test-TMPDIR recipe. R365 retains all 1,769 selected names with seven unexpected
outcomes. The current candidate adds six memory-region production/doc/test
patches; their fresh source and private causal checks pass, while a new normal
build/test run, package and installed consumers remain unqualified. The
[runtime follow-up record](../../verification/llvm19-native-runtime-followup-20261123.json)
binds the exact candidate identities and these observations.

LLDB now follows ordered optional ELF `DT_AUXILIARY` providers when resolving
filter exports. This handles FreeBSD libc's syscall placeholders and libsys
without removing libc from the debugger's module list. The historical R164
selected target exits 1: its 1,761 report entries contain 1,223 PASS, 15 FAIL,
4 UNRESOLVED, 27 XFAIL and 492 UNSUPPORTED. All 1,734 shared names from the
preceding selection remain; 161 FAIL entries become PASS. Its one new
PASS-to-FAIL result is the event test discussed below. The new ELF regression,
fourteen selected original TSan scripts and twelve original MSan dlopen/poison
controls pass. Exact optional `_pthread_` spellings retain the original warning,
stack and negative assertions. Failed dlopen handles are checked before native
linker-map inspection.

FreeBSD Capsicum interception now uses the actual versioned and variadic rights
interfaces. It preserves native validation and short-circuit behavior, checks
inputs to merge/remove operations, and marks only output slots actually written
by `cap_ioctls_get`, preserving its error and `CAP_IOCTLS_ALL` behavior. The
original Capsicum test has a three-slot output buffer matching its requested
capacity. Both original default/named builds pass. Original selected do-test
then exits 0 with 17 PASS and three explicit non-MSan UNSUPPORTED variants of
the new MSan-only regression. All four original Capsicum variants pass, as do
the new rights/ioctl shadow checks and retained dlopen/poison controls.

The event failure is reproduced with the original four test methods and logged
to a stdout notification carrying a cached stopped state. The helper counted
that notification as another stop hook before consuming the next state-change
event. It now matches every primary/shadow notification while counting and
returning only actual state-change events. Every original assertion, timeout
and original test-method body remains. A deterministic data-bearing notification
regression fails against the original helper; the corrected helper passes all
four original methods and the new regression. Earlier failed injection fixtures
and their required-parent gate failures remain recorded. Compiler, debugger,
SDK, source, base and registry guards pass.

The recipe also supplies the ports framework's selected Lua executable:
upstream searches `lua5.3`, while the existing FreeBSD dependency supplies
`lua53`. Selecting the observed interpreter allows the original breakpoint and
comprehensive Lua scripts to finish, then exposes SIGSEGV in
`TestFileHandle.lua`, starting in `TestLegacyFileErr`. That original R267 case
remains failed; its process script was not reached. A subsequent debugger
reproduction identifies `SWIG_TypeCheck` inside `SetErrorFile`: the generic
`SBFile` branch treats native Lua file userdata as a SWIG object before the
exact native-file branch can run.

The first candidate lowers the generic `SBFile` typecheck priority while
retaining its existing conversion and null checks. Generated input, output and
error setters now check exact native Lua file handles first. The original wrapper
compile and shared-library link commands pass in separate output directories,
with only the wrapper input/object and private output/dependency paths changed.
Existing link inputs, warm products, source and SDK remain unchanged. Against
that private candidate, all four unchanged original Lua scripts exit zero and
all sixteen methods are observed, including all three file-handle cases and
both process-launch cases. All final integrity guards pass. This direct-script
control does not replace the normal ports target. An additive upstream method
checks native and wrapped files plus invalid and closed handles for all three
setters. Its separate control exposed another defect: all three setters report
success for already closed Lua files. Lua marks those streams closed through
`closef`, while their `FILE *` may remain non-null. The revised candidate checks
that state before constructing a native file wrapper. Generation and the
original private compile/link commands pass. The revised library passes all
sixteen original methods plus the added contract method, covering all three
setters. Lua and Python independently resolve the same candidate library, and
all final source, existing-product, SDK140, base, registry, mount and test-cookie
guards pass. The normal R327 target now passes this Lua case; package
qualification remains open. Earlier
controller parser and literal-directory comparison failures remain recorded
separately.

Before those two additional Lua patches, R268 original checksum/full patch
passed with zero fuzz. All 160,706 fresh source nodes matched the sealed fresh
baseline except the changed event test and its exact original backup. Every
other body, type, owner, mode, flag and literal link matched. Default options,
dependencies, jobs and test settings were preserved. R313 then passes original
checksum/full patch for the current revision7 origin with zero fuzz. All 160,708 fresh source nodes match R268 except the two
Lua source changes and their exact original backups; all other full metadata,
bodies and links match. All 75 origin Git identities at that point match that
native view; full native read/write permission observations remain separate. R323 then
passes normal revision7 configure/build with fresh owned cookies while preserving
all original top-level cookies. Actual metadata selects the generated wrapper
and toolchain files that require build-actor ownership; their earlier permission
failures remain recorded. The actual cache and generated LIT configuration both
select `/usr/local/bin/lua53`. Fresh stage/package and installed consumers remain
unqualified.

The complete event/Lua failed frontier is independently verified on both
physical filesystems: 1,065,921 nodes, including the full source/build, both
frozen ports views, fresh source and complete SDK140 prefix/raw database. Every
body, literal link, hardlink group and the full gzip stream match; before/after
source and SDK equality pass. This archive predates the separate Lua candidate
probes and preserves their original failed predecessors.
R325 separately retains the normally configured build and all twenty Lua
controls: 1,620,278 nodes and archive SHA-256
`bb02021b19038fecad556733977703ffe18034a5237700c75e4d12c2c5637e95`.
Independent verification on both hosts covers every body, full metadata, literal
link, hardlink group and gzip stream; source/SDK equality and recovery helpers
are verified. It predates the following normal test run.

R327 normal `make do-test` retains all 1,761 original names and adds the eight
Capsicum variants. Its 1,769 entries contain 1,230 PASS, 14 FAIL, 3 UNRESOLVED,
27 XFAIL and 495 UNSUPPORTED. Lua changes from UNRESOLVED to PASS and events
from FAIL to PASS; every other shared status is unchanged. All four original
Capsicum variants and the new MSan case pass; its three non-MSan variants remain
explicitly UNSUPPORTED. Independent body/hash review reconciles JSON with all
1,769 selected XML cases and keeps 84,259 explicitly filtered cases excluded.
The original command exits 1 and its 17 unexpected outcomes remain failures.

Private libc++ compatibility work now reproduces the current base layout
failures against the actual normal debugger and fixes string, vector, map,
unordered-map, forward-list, deque and unique-pointer extraction. The same
unchanged native program verifies all eleven values, including typed map pairs,
sequence values and pointer contents; list/shared-pointer/vector-bool controls
also pass. Checking only counts initially missed empty unordered-map elements.
The revised candidate obtains the actual table value type, following the
[pinned LLVM implementation](https://github.com/llvm/llvm-project/commit/d5e610a6dc8491a9fd6908fb10e4fae65691ed95).
It passes the original five object compiles, archive and both links in private
outputs while preserving all other archive members and ELF dependency contracts.
The actual private CLI mapping and Python binding identities are checked.
All 60 string layout/pair/alternate combinations and five unique-pointer layouts
pass the original simulator assertions, with additional pointer/deleter payload
checks. These twelve source patches are now proposed in the port. R350 passes
the original checksum/full-patch targets with zero fuzz. All 160,720 fresh nodes
match R313 except those twelve declared source changes and their exact original
backups; all other full metadata, bodies, types and literal links match. All 87
current origin Git identities match the native view, and default options,
dependencies and four jobs remain unchanged. Normal R355 configure/full build and R365 selected tests now cover these
additions. The later six memory-region patches still need a new normal run.

The QEMU launch failure also has a native causal reproduction. A 66-byte base
TMPDIR alone yields a working 98-byte socket. Upstream LIT adds its own
`lit-tmp-*` directory and replaces the test environment's temporary paths;
reproducing that behavior produces a 115-byte socket, beyond the actual FreeBSD
104-byte capacity, and the same `Failed to set socket address` error. A shorter
base passes with identical LIT nesting and original emulator exit/argument
assertions. The recipe now allocates a fresh short private directory per run,
exports it through both actor branches, preserves primary-group inheritance and
cleans it on exit. Controls run the actual original recipe with a diagnostic
child: root and unprivileged success/failure cases all keep mode 0700 and correct
ownership/group, bind real nested Unix sockets, remove the allocated directory
and retain child failure47 as make failure1. These controls are separate from upstream qualification. R365 now passes all
nine original QEMU methods and independently verifies cleanup after make failure.

R360 passes original checksum/full patch for the current recipe with zero fuzz.
All 160,720 fresh nodes match R313 except the same twelve libc++ changes and exact
original backups. All 87 current origin Git identities match. Options,
dependencies, build arguments and four jobs remain unchanged; the explicit test
environment change is the runtime-allocated TMPDIR.

R347 now preserves the complete failed normal R327 frontier: 1,620,504 nodes,
archive SHA-256
`3b67cf68656a66dd8c3e22474b11a7ca5ff82fd42092b45a0acd2f878dfae7e9`.
Both physical filesystems independently verify the complete archive, source/SDK
before/after equality and recovery helpers. Original failures remain failures.

R355 normal configure/full default build both exit 0 with fresh owned cookies,
four jobs, all original options/dependencies and 49 build actions. The complete
preceding warm tree and hardlink groups match the verified R347 frontier before
exactly twelve source changes and original backups are integrated. Every final
source/SDK/ports/base/registry/stopped-data/mount/cookie guard passes. R364 then
uses the actual normal CLI and binding, without a private library substitution
or loader override, to verify all eleven current-layout values against the
unchanged native probe.

R365 original selected do-test exits 1 with 1,240 PASS, 5 FAIL, 2 UNRESOLVED,
27 XFAIL and 495 UNSUPPORTED. All 1,769 names remain; ten unexpected results
become PASS with no shared regressions. All 60 string simulator methods, five
unique-pointer methods and nine QEMU methods pass through the original target.
The thread-exit case also changes to PASS, but its cause is not established.
Seven unexpected results remain: GUI spawn-threads, detach-resumes, two memory
search cases, save-core, rerun/expression and Objective-C expressions. Lua,
events and the expected Capsicum outcomes remain passing.

The controller's cleanup review initially assumed a launcher-private variable
would remain in the upstream emulator environment. That assumption fails and
its result is retained. Independent original JSON/XML body/hash/name/status
review passes for all selected cases and keeps 84,259 filter exclusions separate.
Eight actual current-run emulator state files retain standard TMPDIR and identify
one short private root; all socket paths fit the native limit and the allocated
root is absent after make failure. No upstream test was rerun or result rewritten.
Normal product and build-metadata copies were sealed before testing and remain
unchanged, with all final integrity guards passing. Complete current tested-state
archival remains pending; this is not a package or canonical lifecycle receipt.

A separate memory-range diagnosis reads valid heap data while the original
FreeBSD server reports those addresses as unmapped. Its map cache is never
invalidated between stops. A private stop-ID override refreshes it and passes
the unchanged original fixture with the same normal client. A four-stop test
then exposes another defect: mapped PROT_NONE pages lose their mapped identity
because the server omits an empty permissions tuple. The existing client already
distinguishes tuple presence from absence. The revised server preserves that
identity and the documentation clarifies zero-access mapped ranges.

The same compiled mmap fixture and unchanged normal client compare three real
server executables, verified through the inferior parent's kernel-reported path:

| Private server control | Failed four-stop contracts |
| --- | --- |
| Original | Mapped no-access, read/write, read-only |
| Cache refresh only | Mapped no-access |
| Cache refresh and empty-permissions encoding | None |

The fixture checks real mmap/mprotect/munmap results, a target-written byte,
permission changes, unmapped read failure and normal program exit. All original
mapping assertions remain. Original compiler/archive/link commands pass privately
for the two changed objects and archives, preserving every other member and ELF
dependency contract. These object experiments do not replace a normal rebuild.
An additive original API method covers all four states; every original Python
method AST and old C++ body is retained. The new POSIX method alone excludes
Windows. Its actual normal upstream execution remains required.

R382 original checksum/full patch passes with zero fuzz for all 93 current
origin identities. Its 160,726 fresh source nodes match R313 except the eighteen
declared changes and exact original backups; all other recorded full native
metadata, bodies, types and literal links match. Default options, dependencies,
build arguments and four jobs are retained. The six new memory-region patches
still need normal full header-dependent build, original tests and packaging.

These source and selected-runtime checks do not qualify the complete original
suite, fresh package/installed consumers, ARM platforms or the release.
The completed full R74 failure and all remaining defects retain their status.

## Retained native history

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


The complete failed R22 boundary is now independently verified on both physical
filesystems: 551,031 nodes, all bodies and links, twelve inert sockets, six
FIFOs, five hardlink groups and the complete gzip stream. Before/after equality
passes. Its original ASan and MSan huge-allocation binaries each return0 under
their original options. The original UBSan binary instead exceeds the external
256MiB inspection threshold; it is stopped at sampled RSS358872KiB, never a
pass. Its stack reaches libc memset through the main free(p) call. TSan is
already unsupported by the unchanged original source. The complete28-node
diagnostic boundary is independently verified on both filesystems. LLDB obtains
the stack but prints a kill-packet error despite debugger exit0; owned-process
and jail cleanup completes with observed SIGKILL9.

A read-only mallctl probe identifies debug jemalloc5.3.0 with fill and junk
filling enabled by default, retain enabled, no MALLOC_CONF environment entry
and no /etc/malloc.conf. The same original UBSan body and UBSAN_OPTIONS then
complete all64 original iterations in a separate diagnostic process with only
MALLOC_CONF=junk:false added. This fast process exits before any positive RSS
sample, so its recorded sample0 is not a measured process footprint. This
confirms the allocator-option relationship; it is not original-option test
acceptance or a release fix. Global allocator, kernel, SDK and source settings
are unchanged. The intended release-worker profile and testcase preconditions
need review before integration. No case is dropped, allocation size reduced,
full-suite concurrency lowered or diagnostic override adopted as acceptance.


Review of the matching source controller confirms that the worker is following
its intended defaults: [src.conf](https://github.com/jordanhubbard/daemonarchy-src/blob/f4eb2cff7f8b5c511c448f0158f071105d19aaf5/tools/daemonarchy/src.conf)
keeps upstream world defaults and MALLOC_PRODUCTION defaults to no. The source
fixture now declares MALLOC_CONF=junk:false only for the huge-allocation
program. This avoids unrelated debug-allocator filling in the virtual-allocation
regression. All64 allocation sizes/calls, sanitizer options and original skip
annotations remain. No worker/profile/global setting changes.

Fresh original checksum/full patch passes without fuzz. After independently
retaining the failed R22 boundary on both filesystems, only that fixture is
overlaid on its warm source. Original ports do-test/check-all then exits0 with
a disclosed temporary Posix/huge_malloc.c filter. JSON reports ASan/MSan/UBSan
PASS and the unchanged original TSan UNSUPPORTED result. The header has87563
entries; upstream postprocessing reports86020 total,86016 excluded. Those
excluded entries are not claimed as run. Both JSON and xUnit reports are
retained. Source except the single declared fixture, SDK140/rawDB, ports,
registry, base, mounts and stopped-data guards pass. Full test completion
cookies remain unchanged; filtered execution earns no full-suite acceptance.
The separately cold-qualified R24 API changes are not yet integrated into this
warm generation. All-runtime integration, remaining original failures, complete
unfiltered outcomes, packaging and release gates remain open.

The complete new allocator diagnosis, cold source/work/ports and scoped-test
boundary is independently verified on both physical filesystems: 393,529 nodes,
all bodies and literal links, both actual report files and the complete gzip
stream. Before/after equality passes. Selected warm huge-allocation products
and LIT/Ninja state are included; other warm nodes remain bound to the complete
failed R22 archive. This is not a complete replacement warm frontier.

Further native TSan diagnosis reproduces the ignored-library failure with the
unchanged archived R22 executable. A fast-path-only candidate builds but still
fails. The revised FreeBSD thread-exit interceptor instead ends its own scope
before destroying initialized thread state. This balances the interceptor's
ignore state before checking for real unmatched user ignores, and still frees
state in forked children. Fresh original checksum/full patch and full default
configure plus named TSan/CXX/header build phases pass. Source, SDK140/rawDB,
ports, registry, base, mounts and stopped-data guards pass.

Independent native consumers use the unchanged original generated scripts with
only output relocation and a private compiler resource directory. Literal
include/lib links retain normal builtin-header ordering and bind the actual
fresh archives; language-specific driver checks confirm C and CXX routing.
The complete ignored-library script and threaded-fork script each exit0 without
triggering their external inspection guards. The unchanged original negative
user-ignore testcase still emits its required diagnostic and passes its
original upstream deflake/FileCheck checks.

The four-script controller remains failed: compare_exchange still reports five
warnings against its original four-warning check, and deadlock stress still
fails its original ordering check with an additional native libthr race report.
Both match the earlier full-run failures. Earlier consumer controllers also
retain invalid CXX-for-C routing and injected-header-order failures; none is
rewritten or counted as a pass. The new scope-balanced runtime is separately
cold-built; warm source still contains only the allocation-fixture overlay.
The complete separate cold candidate/build and diagnostic boundary is now
independently verified on both physical filesystems: 801,282 nodes, including
both failed and corrected candidates, all fixtures/results, 36 literal links
and the complete gzip stream. Before/after equality passes. Other original
warm input bodies remain bound to the complete failed R22 archive; this does
not replace that warm frontier. Full original-target execution, other runtime/API
integration, remaining TSan and other failures, fresh complete packaging and
release gates remain open.

Native diagnosis of the remaining TSan reports reaches libthr private allocator
memset through pthread_setspecific during libc++ TLS setup. A FreeBSD-only
interceptor/private alias now covers that setter, and nested interception is
scoped to the native setter and mutex-init allocation calls. Application
accesses and mutex annotations remain outside those guards; global sanitizer,
allocator and kernel settings are unchanged. Fresh original checksum/full patch
and full default configure/named TSan/CXX/header build phases pass.

The new regression rejects the preceding runtime: its clean/API mode exits66
and fails CLEAN checks. With the new runtime it exits0 and passes the same
checks. A real application memset race after those calls still passes the
required upstream deflake/FileCheck diagnostics, and the original unmatched
user-ignore testcase still reports its error. Actual links bind the new runtime
archives with normal builtin-header order. All bound inputs remain unchanged.

The original ignored-library, fork and compare-exchange scripts now pass;
compare-exchange retains its original four-warning requirement. The deadlock
script then exposes a separate spelling assumption: FreeBSD can symbolize
_pthread_ aliases at frame zero. Six exact checks, including two negative
stack checks, now permit only that optional underscore. Every original RUN,
C++ test body, warning/cycle/stack/order check and lock mode remains. All nine
RUN commands pass, covering default/second-stack mutex, spin, read/write and
recursive modes. Earlier failed debugger and alias-check results remain
retained. The complete separate source/work/ports/products and diagnostic
boundary is independently verified on both physical filesystems: 794,218 nodes,
42 literal links and the complete gzip stream, with before/after equality.

The API and TSan changes are now integrated into the warm source as five
existing-file changes plus two new regressions, retaining the prior allocation
fixture. Original default all do-build exits0 with all selected components,
options and four jobs. All source nodes except the declared changes, historical
backups/cache, SDK140/rawDB, ports, base, mounts, registry and stopped data pass
integrity checks. This is default-build evidence; full tests, fresh staging,
complete packaging and installed acceptance remain open.

A separate paired original permissions case confirms a recipe ownership defect.
With nobody UID/GID unchanged, a private TMPDIR inheriting wheel group makes the
unchanged FileSystemTest.permissions fail its setgid checks. Giving that TMPDIR
the actor's group makes the same original testcase pass. Both records show one
completed, non-disabled testcase, with unchanged inputs. The original do-test
recipe now retains its no-follow chown flags and sets nobody:nobody. The corrected recipe is frozen in the integrated build view. The selected
original-target run below now verifies its actual directory group and the
original permissions cases.

The complete integrated default-build boundary is independently verified on
both physical filesystems: 551,059 nodes, twelve inert sockets, six FIFOs, five
hardlink groups and the complete gzip stream. Before/after equality passes.
Historical stage/case products are preserved; they are not fresh package
acceptance.

The subsequent original do-test/check-all completes its disclosed temporary
selection with 1,435 PASS, 2 FAIL and 1 unchanged original UNSUPPORTED in JSON.
Initial LIT discovery has 87,566 entries and selects 74 entries including GTest
shards; postprocessing reports individual cases. Unselected entries are not
claimed as run. Both actual JSON and xUnit bodies are retained. Original make
exits 1, so this target is still failed. Source, SDK140/rawDB, ports, base, mounts,
registry and stopped-data guards pass, the owned jail is removed, and no full
suite completion cookie is created.

The complete failed selected-test boundary is independently verified on both
physical filesystems: 551,067 nodes, including twelve inert sockets, six FIFOs
and five hardlink groups. Every archived body and the complete gzip stream
match the inventory; before/after frontier equality passes.

Actual build/HOME/TMPDIR groups are nobody (65534). The unchanged permissions,
no-execute-directory removal and both objcopy/dwarfutil mirror-permissions tests
now pass, as do the selected repaired API/allocation/fork and TSan scripts and
regressions. Two selected unit cases remain failed:
SignalsTest.PrintsSymbolizerMarkup omits module/map records, and
Posix.ThreadSpecificDtors reports an unexpected TSan diagnostic. Other original
failures outside this temporary selection, complete unfiltered outcomes, fresh
staging/package/installed acceptance and release gates remain open. The initial
input-preparation status-label rejection is preserved; it occurred before any
native test producer was created.

The next CMake follow-up resolves both selected unit failures without changing
original test bodies or assertions. SupportTests lacked the GNU build ID that
printDSOMarkup requires before emitting module/map context. Its FreeBSD target
now links with --build-id. The TSan unit executable was byte-identical across
R22, the runtime rebuild and the failed selected run: the common test generator
discarded declared dependencies before linking, while the aggregate tsan target
provided ordering alone. The generator now retains declared link dependencies;
non-Apple TSan units name both concrete per-architecture runtime archives.

Fresh original checksum/full patch passes without fuzz. Original default all
and named SupportTests/TsanUnitTests builds pass with unchanged components,
options, dependencies and four jobs; all final integrity guards pass. The new
Ninja rule has both actual archives as file inputs, the unit binary contains
the new setter interceptor, and SupportTests has a GNU build-ID note.

Original selected do-test/check-all now exits zero with 1,437 PASS and one
unchanged UNSUPPORTED. Independent comparison of both JSON bodies verifies the
same 1,438 case names as failed R63: only the symbolizer and destructor cases
change from FAIL to PASS. Actual JSON/xUnit bodies match recorded hashes; all
final SDK140/rawDB/source/ports/base/mount/registry/stopped-data guards pass.
No full-suite test cookie is created. The complete current frontier is
independently verified on both physical filesystems: 711,782 nodes, including
cold original source, integrated source/build/historical stage, fixtures,
controls and both reports. All bodies, 256 literal symlinks, twelve inert
sockets, six FIFOs, five hardlink groups and the complete gzip stream match;
before/after equality passes.

The subsequent unfiltered original R74 do-test/check-all completes with make
exit 1 and both structured reports. It starts with 87,566 LIT entries and
reports 117,174 entries after expansion of GTest cases: 111,246 PASS,
5,410 UNSUPPORTED, 269 XFAIL, 3 XPASS, 239 FAIL, 4 UNRESOLVED and
3 TIMEOUT. Initial discovery entries, console shards and final report entries
are different counting units. The 249 unexpected entries remain failures to
resolve; this is a completed failing run. Most are in LLDB API/Shell, with
additional sanitizer, CFI, libFuzzer and OpenMP failures. Existing explicit
skip reasons and expected failures remain recorded in the reports.

All final source, SDK140/rawDB, ports, base, registry, mounts and stopped-data
guards pass; the owned jail is removed and full-test cookies are unchanged.
The complete 589,383-node failure frontier is independently verified on both
physical filesystems, including all regular bodies, 260 literal symlinks,
twelve inert sockets, twelve FIFOs, six hardlink groups and the full gzip
stream. The JSON and xUnit report hashes are recorded in the verification
record. This does not qualify the historical stage tree as a fresh package.

Nine fixture corrections then pass fresh original checksum/full patch without
fuzz. They propagate the configured shell to LLDB API/Shell tests, select the
existing FreeBSD lldb-server for DAP, correct the anonymous-mapping fd, and
check the platform's exact loader diagnostics. Passing dynamic-ASan and x86_64
MSan DTLS variants must now pass; static ASan retains its demonstrated XFAIL.
Original compile/program runs and behavioral checks remain; the OMPT
FileCheck invocation selects the platform diagnostic prefixes.

Original R100 do-test/check-all completes an explicitly temporary selection
of all LLDB API/Shell tests and the corrected sanitizer/CFI/OMPT cases. The
same 1,747 names appeared in R74. Results are 1,040 PASS, 179 FAIL,
4 UNRESOLVED, 28 XFAIL and 496 UNSUPPORTED, with original make exit 1.
Thirteen FAIL, three TIMEOUT and three XPASS entries become PASS. One
previously passing thread-exit testcase becomes FAIL and remains unresolved;
it is not discarded or classified as flaky. All final integrity guards pass,
the jail is removed and test cookies are unchanged.

The CFI fd correction lets all four supported shadow tests complete their
original mmap RUN. Their later dlopen RUN still fails before its expected
crash marker. Four other variants retain UNSUPPORTED; complete CFI success
is not claimed. The new full run remains necessary after all fixes.

The complete 749,919-node R99/R100/R108 frontier is independently verified
on both physical filesystems, including cold and integrated source, build,
historical stage, controls, reports, all regular bodies and literal links,
twelve sockets, twelve FIFOs, six hardlink groups and the full gzip stream.
The R108 diagnostic reproduced the expression failure but used an invalid
expression logging category; it is retained with that limitation. Later
separate diagnosis does not replace the failed original run.

These checks establish neither full upstream suite success nor a fresh full
package build with the changed recipe. The complete R74 reports provide discovery and per-entry outcomes; diagnosis
and resolution of the unexpected results remain open. Independent installed compiler consumers, final
CURRENT integration, complete toolchain provenance, canonical Component/SDK/
lifecycle admission and full 1.6 release acceptance remain open. Raw worker
evidence stays private.
