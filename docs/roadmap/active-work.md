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

### Native libuv kqueue poll repair

The original FreeBSD ARM64 `poll_nested_kqueue` test fails because both
nonblocking operations reject the kqueue descriptor with `ENOTTY`. The candidate
checks that descriptor using a zero-event, zero-timeout `kevent` call before
accepting this specific failure. Native controls preserve a pending event and
reject five other descriptor cases. Seven original poll cases pass; the two
Windows-only cases keep their original skips. Test code and filters stay unchanged.

The [native observation](../verification/libuv-freebsd-kqueue-20261124.json)
binds the original and candidate source bodies. The probe relinks original test
objects; it is separate from a fresh complete port build. The full unfiltered
runner invokes all 451 cases: 436 pass, 12 keep original skips, and three
pre-existing connect/UDP failures remain. Network-policy and IPv6 fixture
qualification is still needed.
AMD64, independent packages, normal installation and canonical lifecycle/SDK
acceptance remain open. No kernel or network policy is changed by this repair.
