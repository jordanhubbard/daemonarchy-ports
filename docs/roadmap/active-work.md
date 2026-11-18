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

### [ ] PORT-HERDR-001 — Bind the required Zig compiler series

- **Owner:** sysutils/herdr and its vendored libghostty-vt build.
- **Direction:** Retain the original default build and bind its actual compiler requirement through the shared Zig framework.
- **Implementation:** Select `USES=zig:016` and bump PORTREVISION. The current unversioned Zig port supplies 0.17, while the vendored Ghostty build requires 0.16 and fails with removed Build API members under 0.17. The existing `lang/zig016` origin supplies the required compiler at the existing command path.
- **Evidence:** The original default AMD64 source99/ports2ba0 bulk reached the actual vendored Ghostty compile and failed with both the version requirement and missing `Build.build_root`. The complete failed log and frozen port inputs are preserved on two healthy hosts. Actual native FreeBSD metadata resolves the original compiler dependency to `lang/zig` 0.17 and the candidate to `lang/zig016` 0.16, retaining the original Cargo flags, four jobs and options in the isolated view. A clean source-built compiler/package build, original runtime controls and the complete selected closure still require qualification; no passing build or SDK/lifecycle receipt is implied by this repair.

### [ ] PORT-DESKTOP-002 — Qualify current desktop package snapshots

- **Owner:** x11-wm/omarchy and the retained FreeBSD adapter.
- **Direction:** Package the reviewed current desktop and native adapter without treating their focused tests as complete release acceptance.
- **Implementation:** Select Omarchy `853c91cea133c203fe199329b85278dc213ccf9c` and adapter `0c139ec87394775ea948a8ad945c9a8035ef3485`, retaining the existing dated package version with PORTREVISION 1. Update the network patch to preserve upstream's `switchingNetwork` term and the existing FreeBSD NetworkManager guard. Refresh the package inventory for the exact added/deleted sources and native adaptation, including the translation watcher and privileged firmware helper.
- **Evidence:** Both complete source archives match every Git blob, mode and path (5,709 desktop and 87 adapter files) and are verified on two healthy hosts. Original native checksum/extract/patch/FreeBSD adaptation succeeds in a fresh private view after the one network hunk rebase; the first failed patch, reject and complete workdir remain preserved. The first observer's environment-directory override mistake was stopped and preserved, then corrected with native make command-line variables and explicit directory checks. Original port files and host/jail package inventories remain unchanged. The adapted firmware helper has its native Bash interpreter with `-p` retained. The candidate package list follows the original install mapping; actual native build/package/staging/check-plist/install, installed helper/policy and GUI behavior, complete current source/ports closure and per-unit SDK/lifecycle acceptance remain required.

The first actual native desktop package run resolves its original dependencies,
verifies both source checksums and reaches staging, but developer QA rejects
three new script interpreters. Extend the original shebang selection to the
console-font Python script and app scaffold. Preserve the firmware helper's
`-p` flag when rewriting the retained dev-link command copy. The normal main
command was already rewritten correctly. Keep developer stage QA, package,
plist and install checks enabled. Preserve the complete original selected work
directory on both success and failure in subsequent native workflow artifacts;
the first run's complete uploaded raw logs remain retained. Original native
package/staging/plist/install qualification still requires a fresh corrected run.
