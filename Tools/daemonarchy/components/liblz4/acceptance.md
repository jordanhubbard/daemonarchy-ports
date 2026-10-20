# Original LZ4 port acceptance

- Verify exact origin/patch/Mk/distinfo/distfile and toolchain/ABI/options inputs.
  Refuse a missing dependency, checksum mismatch or attempted live dependency
  installation. Preserve original licenses and all selected upstream files.
- Derive the actual original registration inventory from the selected upstream
  test target. Retain observed outcomes from newly built products; do not create
  success reports from an exit code or suite label alone. TEST off selects the
  original check target; TEST on selects its declared extensive-test closure.
- Independently test sealed library and command products with exact content,
  buffer-bound, corrupt/truncated-frame and command-failure expectations.
- Reopen native pkg bytes, verify version/ABI/origin, payloads, both BOMs and
  actual source/build/dependency provenance. Install/update/rollback only in the
  disposable acceptance environment.
- Repeat unchanged work and separately change implementation, patch, shared Mk,
  options, dependency and compiler. Prove correct affected invalidation; old
  completion cookies and fast no-op returns cannot establish correctness.

The product repository's direct make probe is diagnostic evidence. It is not a
passing execution or pkg acceptance receipt for this Component.
