# Draft retained profile (daemonarchy-ports): provenance and unknowns

DRAFT, unqualified. Nothing here asserts a build, test, acceptance or release result. The
`authority-reviewed` marker and every `--authorize-*` flag belong to the operator and were not
used. Regenerate on the amd64 builder (mudflap), from the project root:

1. `python3.12 Tools/daemonarchy/retained/closure-inventory.py --spec Tools/daemonarchy/retained/closure-spec.json --output Tools/daemonarchy/retained/closure-inventory.json`
2. `python3.12 Tools/daemonarchy/retained/stage-closure.py --inventory Tools/daemonarchy/retained/closure-inventory.json --root .`
   (untracked `closure/`; add it to `.git/info/exclude`)
3. `litai-venv/bin/python Tools/daemonarchy/retained/draft-retained-inputs.py --manifest-out /abs/outside/tree/manifest.json`

## Changes from the first draft
- Toolchain closure is real: 94 captured tools (`poudriere` and the helper commands its
  bulk/queue/ports/common/include scripts invoke, `python3.12`, `git`, `pkg`), 48 shared libraries
  (observed with `ldd`), the Python stdlib subset the report tools import, poudriere `share/` and
  `libexec/`; owners, sizes, modes and digests are in `closure-inventory.json`. The old `sdk/bin`
  held three copies (`python3.12`, `poudriere`, `true`) and no libraries.
- Dependency closure is real: `gmake` 4.4.1 and `pkgconf` (the build requirements
  `docs/user/ports-units.md` names) are captured under `closure/dependency` as non-tool inputs
  (the framework only admits toolchain-role files as commands). The profile's
  `runtime_library_paths` covers both library directories.
- Ambient host inputs that cannot live in the captured tree (the registry, build-jail anchors,
  git's compiled-in helper directories, the dynamic linker, the `/bin/sh` interpreter of the
  `poudriere` script) are recorded by identity and re-observed by `draft-ambient-preflight`.
- Output paths: the first draft wrote `retained-out/...` relative paths, which would land in the
  working directory (the input snapshot) rather than the sealed output. All outputs now use
  `{output_root}/...` as in the other two repos, and the test policy's report path is
  `tests/unit-report.json`. The `true` placeholders are gone.
- Docs: `draft-docs-tree` checks the 19 project-authored Markdown files (`docs/`, root, `Tools/`).
- Accept: `draft-accept-packages` (`accept-packages.py`) re-observes the sealed package set
  separately from poudriere.

## Jail and tree names, and the output mapping
`run-bulk.sh OUTDIR` (with `bulk.env`) copies the reviewed poudriere registry from
`/usr/daemonarchy/rev2-20261030/poudriere-etc` into the run's private TMPDIR (the shared registry
is never written), points `POUDRIERE_DATA` and `DISTFILES_CACHE` at `OUTDIR`
(`{output_root}/build`), registers the retained snapshot itself as ports tree `retained`
(`ports -c -m null -M <snapshot>`), and runs
`poudriere -e <etc> bulk -j 16r2303b36 -p retained -b latest -t -f origins.draft.list`. The sealed
package set is `OUTDIR/poudriere-data/packages/16r2303b36-retained`.
Evidence for the names: HACKING.md's examples `16amd64`/`omarchyiso` are NOT registered on mudflap.
The registry holds jails `16fin06e5be 16live6bac4c 16live7da95f 16live8f6f30 16macd63d77
16r2303b36 16r2303b36s` (only `16r2303b36` and `16r2303b36s` have a root under
`/usr/local/poudriere/jails`) and ports trees `brcmfmac131fix daports141 daports31 daportsmain
macbookefb8` (all null-method). `16r2303b36` is the release jail; `16r2303b36s` is the sibling
with kernel source for module builds (`docs/mastering-1.5.0.md`).

## Retained execution and root
`poudriere` mounts and jails, so `run-bulk.sh` refuses a non-root user. The retained executor has
no general root refusal (only the optional FreeBSD `secondary_access` views and ACL custody need
root and a disposable-allocation inspector; unused here), so the action can run as root if the
operator runs `litai build ... --allow-host-execution` as root; the framework states its
workspace is not an OS sandbox. `observe-unit-inputs.py` (not an action) requires a non-root user;
its unit test patches `geteuid`. The src repo's run must be unprivileged and cannot share this one.

## UNKNOWN, with the evidence examined
- Whether the release jail `16r2303b36` is the intended validation jail for the ports fork
  (HACKING.md names another). Whole-jail identity needs root (root-only directories); only
  `param.h`, `libc` and the jail `cc` are anchored.
- Whether `poudriere ports -c -m null` over the retained snapshot and a registry copy behave as
  expected with `-b latest -t`; nothing was run. Where poudriere builds (BASEFS
  `/usr/local/poudriere`, `.m` mounts) and whether it writes outside `OUTDIR` is unobserved.
- Network: `-b latest` fetches official packages from pkg.FreeBSD.org and the port's distfiles
  are fetched into an empty `OUTDIR/distfiles` (checked against `distinfo` by poudriere); neither
  fetch has a retained identity. The run needs network access.
- Poudriere's complete helper-command set (the 94 tools come from command-position grep of its
  scripts, not a trace). `run-bulk.sh` appends `/sbin:/bin:/usr/sbin:/usr/bin` to PATH for anything
  missed, so the closure is not hermetic. `share/poudriere/MANIFESTS` (absolute symlink to
  `/usr/local/share/freebsd/MANIFESTS`, used by jail creation) is not captured.
- gmake/pkgconf in the jail: the jail resolves its own copies; whether they equal the captured
  host copies is unknown. The compiler and sysroot are the jail's, outside the captured tree.
- Scope: only `archivers/liblz4` is built (the first closure in `ports-units.md`); a whole-tree
  or per-origin build is not defined by any document. Which origins constitute acceptance is the
  operator's decision.
- Docs: no ports docs build exists; port Makefiles, `pkg-descr` and `Mk/` have no documentation check.
- Acceptance: independent sealed-product acceptance is stated open in `ports-units.md`; no
  acceptance policy exists. `draft-accept-packages` is a static check only (one package per origin,
  readable archive with `+MANIFEST`, declared dependencies present) and says nothing about
  installation or the port's own tests.
- Tests: only `test_observe_unit_inputs.py` (16 cases, what CI runs) is inventoried; the native
  hosted-FreeBSD step and `test_omarchy_debug_status.py` are not. Nothing was run through the
  lifecycle; any skip would be refused (every `skip_reason` is null).
- Release policy: tag prefix, signing and provider remain proposals copied from daemonarchy-src.
