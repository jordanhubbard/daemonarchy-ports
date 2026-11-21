# Native ports development

### [ ] PORT-UNIT-001 — Qualify original port and shared make composition

- **Priority:** P1
- **Owner:** Original port contracts and retained native lifecycle
- **Direction:** Unify source and ports build targets through installed litai-dev in the parallel workspace, preserving regular upstream imports.
- **Conclusion:** Begin with the real liblz4 origin and shared ports make support; keep registration, native probes and admitted per-unit products distinct.
- **Depends on:** Daemonarchy DEV-001, COMP-001 and PERF-001; framework retained native lifecycle issue 22
- **Implementation:**
  - [x] Author canonical liblz4 and shared framework contracts, exact FreeBSD/pkg locks and reviewed original-source authority without moving implementation.
  - [ ] Bind exact selected port/Mk/distfile/patch/options/bootstrap inputs and the full installed Standard policy to executable retained profiles.
  - [ ] Implement and qualify complete-tree scaling through the canonical framework. Exact requested limits remain in Tools/daemonarchy/retained/project-scale-limits.requested.json; the installed manifest schema does not support activating them.
  - [ ] Expose installed build/test/package execution and original outcomes plus independent sealed-product acceptance; forbid ambient dependency installation.
  - [ ] Prove clean/warm/source/Mk/options/toolchain invalidation and actual pkg BOM/provenance, then disposable VM installation/update/rollback.
- **Evidence:**
  - [x] Current project authority, documentation review and both exact FreeBSD/pkg locks validate through the installed canonical wheel at framework revision `5b9afcd7c9016341620695b9110ec073809d239c`. Standard binding preserves all sixteen required evidence kinds. Verification fails solely on the missing current test receipt; native execution is not admitted.
  - [ ] The installed CLI executes the original build, complete selected upstream tests and independent acceptance, and admits per-unit pkg products with actual provenance.
  - [ ] Equivalent native measurements and upstream-refresh/cut-over/rollback evidence satisfy the product requirements.

The reviewed installed-wheel rebind plan
`sha256:633d5a210fbf887155ffb855b6dab5c242bbbf78b29054bcc3a77e1f94fb8f5e`
selects the common retained build/test frontend while preserving all sixteen
evidence kinds and the Standard policy. Native deployment and executable
profile/toolchain admission remain the next work; this rebind grants no receipt.

### [ ] PORT-T2-001 — Reconcile and qualify the retained Touch ID software

- **Owner:** security/t2-touchid and libfprint virtual-device integration.
- **Direction:** Preserve the previously reviewed legacy integration in owned main and qualify its actual FreeBSD software behavior.
- **Implementation:**
  - [x] Merge the legacy PR33 port and libfprint VIRTUAL option without replacing current category or changelog entries.
  - [x] Verify the checksum-bound source archive and apply all retained FreeBSD patches with zero fuzz.
  - [ ] Build/package/install the selected ports in the disposable native worker and inspect every original test outcome.
  - [ ] Qualify the matching CURRENT package closure and independent virtual-fprintd/software failure behavior.
- **Evidence:** Native phase logs and package manifests remain separate from canonical Component/SDK/lifecycle receipts. No physical Touch ID success is claimed; physical sensor acceptance is outside this execution.

### Current Omarchy snapshot for SYNC-001

The dated desktop snapshot selects Omarchy
`0640b8c1c761fd962298907712596c1197c13a29` and the FreeBSD adapter
`029e0eba6ff69392e416abb5bbd58206d4d20443`. Their original source archives
match every Git blob, executable mode and path: 5,719 desktop files, including
six literal symbolic links, and 87 adapter files. Each selected tree is
identical to its reviewed reference/native compatibility test head.

The port carries the 179 built-in icon templates used by the installed icon
selector, alongside the refreshed source packing list. Its native installed
command control runs as nobody with an empty user icon directory and requires
Arcade, Papercraft and Original to remain discoverable. Original source,
staging, products and failure statuses remain retained by the native driver.

This selected-port workflow uses ordinary binary dependency bootstrap and a
source-built libdvdcss dependency on FreeBSD 15.1 amd64. It does not establish
the full matching-CURRENT source-built closure, graphical session, physical
hardware, canonical per-unit SDK or retained lifecycle/release acceptance.

### [ ] PORT-MONO-001 — Qualify ARM64 Mono bootstrap compiler serialization

- **Priority:** P1
- **Owner:** lang/mono5.20 native ARM64 port
- **Direction:** Complete the existing ARM64 Roslyn serialization at direct bootstrap compiler calls without reducing default build concurrency or dependencies.
- **Conclusion:** The original full selected ARM64 closure crashes in Mono and skips four dependents. Two direct bootstrap calls omit the existing serialization flags; the candidate is not yet natively qualified. Product issue #333 tracks this work.
- **Depends on:** Daemonarchy issue #333 and COMP-001
- **Implementation:**
  - [ ] Extend the existing aarch64-only patch to both direct BOOTSTRAP_MCS calls and bump PORTREVISION; preserve all original options, distfiles and four make jobs.
- **Evidence:**
  - [ ] Preserve the original native SIGSEGV log and exact source/patch identities; qualify a full default-options native build/package, original tests and all skipped dependents before complete selected-closure and release acceptance.

The native source99 diagnostic builds of both the original revision and the
bootstrap serialization candidate completed, so the original source97 build
crash is not yet reproduced under that newer kernel. The candidate's original
`make test` (`check -k`, four make jobs, default X11 and all test dependencies)
failed: Roslyn compilation crashed, some upstream test inputs were missing,
and the sampling profiler aborted after `clock_nanosleep` returned EOPNOTSUPP.
The complete failed work directory, cores, logs and reports are retained on
healthy storage. These failures remain release qualification gaps.

A separate native clock probe reproduced the profiler's incorrect selection of
the unsupported process CPU sleep clock. A follow-up port patch selects it only
after a successful probe and retains the existing monotonic clock fallback.
The fallback passed the standalone native probe. Rebuilding the actual runtime
with that one source change then passed all nine unchanged original profiler
checks, including the previously aborting sampling case. The original port
source, all 174 cached package bytes and the host guards remained unchanged in
that declared runtime experiment; both its closed evidence and the earlier full
failure archive are verified on two healthy hosts. The remaining original tests,
a complete revised-port build and the affected closure still require
qualification. No complete Mono test or causal bootstrap crash-repair pass is
claimed.

The same diagnostic worker also reproduced the original test compiler's
SIGSEGV/abort once in three invocations of the unchanged `builtin-types.cs`
input. Adding only `/parallel-` compiled all three paired invocations; all five
produced executables each passed their 80 original runtime cases. The ARM64-only
workaround now also covers the direct compiler definitions in `mono/mini` and
`mono/tests`, retaining four make jobs and the complete test selection.

The release tarball omits three required test sources and seven xUnit binaries.
Restore those inputs from the exact original Mono 5.20.1.34 tag commit
`feca91f7eb1b440a6520cf754370b98271339d1a` and its xUnit submodule commit
`bb583470c7fc9f5b8369a4c273176c8202f00b1a`, with checksum-bound distfiles.
These upstream prebuilt test tools do not establish source-built SDK provenance.
The complete revised original test target and a clean revision 7 port build
remain qualification requirements.

The retained-workdir follow-up completed with 249 passing cases in five XML
reports, but its full target still failed on further omitted test inputs. Its
generated compiler rules also remained old under disabled maintainer mode, so
it does not qualify the new compiler flags. An expanded audit found 3,472
missing original-tag test files (about 16 MB), including the NUnit launcher,
shared test helpers and runtime fixtures. A revision 8 follow-up restores the
original test trees without overwriting existing files or downstream patches.
The existing monodoc test files match the original api-doc-tools submodule;
restoring the test directories makes their original relative paths resolve.
A clean original autoreconf/configure/build/package and the complete revised
test target remain required; the failed workdir and raw reports are retained
and verified on two healthy hosts.

The clean revision 8 native build completed and activated the intended compiler
flags. Its full original test target still failed: the gc-descriptor compiler
has a separate definition without the ARM workaround; FreeBSD's empty AOT
suffix aliases AOT recipes to ordinary DLL targets; and six original-tag test
inputs remain absent outside the previously restored trees. The retained run
includes 27,914 passing mini test executions across 23 optimization combinations
and 293 passing XML cases, but these do not qualify the complete target. All
original reports, the compiler core and the complete workdir are preserved on
two healthy hosts. The produced revision 8 package is also preserved separately;
the first observer checked only an absent convenience repository path.

Revision 9 extends the existing ARM compiler workaround to every remaining
Mono test compiler definition, assigns the ELF AOT suffix on FreeBSD and
restores the remaining original-tag resource/remote-executor/xbuild fixtures
without overwriting source. Default options, four make jobs and all original
test groups remain selected. Complete native patch/extraction checks, a clean
revised build and the full upstream test target remain required.


The fresh revision 9 build on the complete current ports source tree first
rebuilt all 21 stale test-support dependencies successfully, but failed before
packaging or upstream tests during compiler AOT generation. Giving FreeBSD its
proper `.so` AOT suffix exposes an original GNU Make whitespace bug: two compiler
profile options expand into separate arguments, and Mono treats the second
`,profile=...` argument as an assembly path. Actual native GNU Make 4.4.1 and the
two unchanged source profile files reproduce that argument split. Defining the
separator as one literal space preserves both profiles in one AOT option.
Revision 10 applies that narrow original Makefile correction and keeps all AOT
work, default options, four jobs and original tests selected. The failed full
revision 9 work directory and all output remain retained; full revised native
build, package, original tests and affected current closure are still required.

The isolated actual compiler follow-up reproduces the original malformed
argument failure. With the separator corrected, Mono reads both unchanged
profiles and compiles all 132 selected compiler methods, then fails because
the default native jail has no `as`. Bind the real GNU assembler through the
shared binutils support, selecting only AS and retaining the base compiler and
linker; retain it as a runtime dependency for `mono --aot` too. The next native
control uses a verified source-built binutils package in a private directory,
with no installation on the live worker. Full port/package/test and current
closure qualification remain required after these focused controls.

The corrected native AOT control now completes with both original profiles:
all 132 methods compile, GNU assembler 2.44 assembles the output and the base
linker produces an ELF64 AArch64 shared image. The original runtime, compiler,
mscorlib and profile inputs remain byte-identical, and host state is unchanged.
The source-built assembler and Zstd dependency were extracted into a private
directory; no packages were installed on the live worker. The original argument
failure and missing-assembler failure remain preserved alongside the successful
control. Native ports metadata confirms only AS switches to binutils; base
CC/CXX/LD, default MONOLITE/NLS/X11, four jobs, the six test-support dependencies
and the full `check -k` target remain selected. These focused checks do not
qualify a complete revision 10 build, package, upstream tests or release closure.

The complete revision 10 build gets past AOT argument assembly and all six
compiler AOT image generations, but aborts when Roslyn starts to build mscorlib.
The ARM64 PLT decoder expects ADRP/ADD; FreeBSD LLD 21 relaxes nearby sequences
to NOP/ADR. The original core, all runtime/compiler/AOT inputs and complete
failed workdir are retained on two healthy hosts with source/cache/host guards.
An actual native control generates each of the six assembly objects once and
links those same objects both ways. Default linking reproduces the exact
trampoline assertion; adding only `--no-relax` preserves every PLT entry and
runs the compiler with all six AOT images loaded. All original input bytes stay
unchanged. Revision 11 selects that native AOT linker flag on FreeBSD ARM64,
preserving AOT, the base compiler/linker, four jobs, profiles and original tests.
A clean revised port build, package and full original upstream tests remain
required; the successful controlled compiler startup is bounded evidence.

The clean revision 11 native ARM64 run passes the previous Roslyn AOT startup
failure and reaches staging, but its original `check-plist` gate refuses seven
unlisted compiler AOT images. All seven staged files are independently reopened
as ELF64 AArch64 shared images. The testport launcher returns zero despite the
recorded port failure; the independent failed-row check correctly stops before
upstream tests. The complete retained workdir and raw logs are preserved and
verified on two healthy hosts, and all 184,085 source inputs, 176 original seed
packages and host guards remain unchanged. No revision 11 package or full-test
acceptance is claimed.

Revision 12 explicitly lists those seven existing payloads on the FreeBSD
targets whose upstream configure enables system AOT by default: aarch64, amd64
and i386. Other supported targets keep their original selection. AOT generation,
compiler/linker choices, default options, four jobs and all upstream test groups
remain selected. Native metadata, a clean revised build/package and the complete
original upstream tests remain required; the package-list correction does not
qualify the selected closure, SDK or release.


The complete clean revision 12 ARM64 package build passes, including check-plist
and all seven compiler AOT images. The actual 90,239,621-byte package has SHA256
`00a04ff4fa9b18f4af6ef019cd5209bf0f2d20fe9d93cb9b616b6d94472854a7`.
Its original full `make test` / `check -k` command then runs for 37 minutes and
exits 1. All source inputs, 176 seed packages and host postguards pass. The full
workdir, raw logs and all 114 retained report products are independently archived
and verified on two healthy hosts. Of those products, 107 XML reports retain
46,628 case records, including failures, errors and original ignored outcomes.
Repeated upstream profile runs can overwrite reports; these counts are not a
claim of complete unique coverage, and no failure or ignored case is waived.

The failed run exposes missing libgdiplus and SQLite test libraries, GNU patch's
`--binary` requirement in the original mdoc tests, and an upstream runtime rule
named `mcs-do-xunit-run-test-profiles` while check invokes
`mcs-do-run-xunit-test-profiles`. Revision 13 declares the three test packages,
lets mdoc select GNU patch through the test environment and makes that existing
xUnit recipe reachable under its caller's name. Original test groups, default
options, four jobs and native build tools remain selected. Other retained test
failures still require diagnosis; these prerequisite and rule corrections do
not establish a full-test pass, affected-dependent qualification or current
source/ports/SDK/lifecycle/release acceptance. A fresh clean native build and
the complete original target remain required.

### FreeBSD update snapshot configuration (Daemonarchy issue 334)

The current desktop still lets the historical snapshot-service migration reach
Linux Snapper setup on FreeBSD. The port now guards that shared setup leaf by
operating system. FreeBSD keeps the existing `omarchy-snapshot` boot-environment
backend; Linux keeps its original Snapper configuration and timer operations.
The historical migration and the runner's completion markers and stop-on-failure
behavior are unchanged. The package revision increases to distinguish the fix.

Private shell controls reproduce the original missing-Snapper exit 127, then
complete the migration with the patched leaf. Another migration's exit 17 still
stops the runner without writing its completion marker. Executed and sourced
leaf controls pass, and Linux configuration bytes, modes and service arguments
match the original. Native FreeBSD ARM64 controls also reject the old leaf and
accept the patched leaf as nobody, without invoking Snapper or systemctl or
creating their configuration files. The native driver retains that check for
the actual installed file. These source controls do not establish a new package,
an installed-product pass, a full desktop update or lifecycle/release acceptance;
the revised native package and installed checks remain required.
