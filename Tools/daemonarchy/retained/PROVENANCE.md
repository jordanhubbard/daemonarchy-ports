# Draft retained profile: provenance and unknowns

DRAFT, unqualified. Nothing here asserts a build, test, acceptance or release result.
Regenerate with `draft-retained-inputs.py` (see its docstring) after staging `sdk/bin`
and `deps/bin` untracked on the worker.

## Grounded in the repository and its documents
- Build action: `poudriere bulk -j 16amd64 -p omarchyiso -b latest -t -f <list>` is step 4
  of "Build the packages" in the daemonarchy HACKING.md; the origin list holds only
  `archivers/liblz4`, the first closure in `docs/user/ports-units.md`.
- Test action: `Tools/daemonarchy/test_observe_unit_inputs.py`, the command the
  repository's `producer-inputs-native.yml` runs, driven by `unittest-report.py` so the
  retained report records outcomes observed at run time.
- Dependency closure: `gmake` and `pkgconf`, the build requirements `ports-units.md`
  says the native make probe selected (`gmake>=4.4.1`, `pkgconf>=1.3.0_1`). Captured from
  the mudflap host packages `gmake-4.4.1` and `pkgconf-3.0.9,1` (`/usr/local/bin`).
- Toolchain tools: `python3.12` (report producer, `python3.12` package on mudflap),
  `poudriere` (`/usr/local/bin/poudriere`, `poudriere-devel-3.4.99.20260425`) and
  `true` (`/usr/bin/true`, placeholder tool for the UNKNOWN stages).
- Release policy: `literate.release.json`, modeled on daemonarchy and daemonarchy-src.

## UNKNOWN, with reason
- Jail `16amd64` and ports tree `omarchyiso`: names are HACKING.md's examples; whether
  they are registered on the worker was not inspected (poudriere is out of bounds).
- How poudriere results (its own `data/packages`) map into the retained output root
  `retained-out/build`, and how its registered ports tree relates to this retained tree:
  no document defines it.
- Toolchain closure completeness: the scripts and interpreters' shared libraries, Python
  stdlib, poudriere's `share/` and the jail's compiler/sysroot are not captured.
- Dependency closure: the poudriere jail resolves its own gmake and pkgconf; whether they
  equal the host copies captured here is unknown.
- Docs action: no ports docs build or check is defined anywhere in the repository or docs.
- Acceptance action and policy: independent sealed-product acceptance is stated open in
  `ports-units.md`; no acceptance policy exists.
- Test outcomes and skip reasons: not run. The native hosted-FreeBSD step and
  `test_omarchy_debug_status.py` are not in the inventory because CI does not run them.
- Release policy tag prefix, signing and provider: proposals copied from daemonarchy-src.
