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
