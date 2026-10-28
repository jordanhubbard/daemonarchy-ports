# Daemonarchy ports changelog

## Unreleased

- Add `security/t2-touchid`, KaiT2en's Apple T2 Touch ID bridge for fprintd
  (upstream commit `16b58080ebc8`, GPL-3.0-or-later with its attribution
  term), patched for FreeBSD and disabled by default, and a default-on
  `VIRTUAL` option in `security/libfprint` that builds the virtual storage
  device it drives. The MacBookPro16,1 transport test reached the Secure
  Enclave; login by finger needs macOS-enrolled fingers and remains untested.

- Rebind reviewed authority to framework revision
  `5b9afcd7c9016341620695b9110ec073809d239c`, preserving the complete Standard
  policy and exposing the common retained build/test frontend. Native execution
  and package qualification remain open.

- Add canonical contracts for the original `archivers/liblz4` origin and shared
  ports make support, with exact FreeBSD/pkg locks and inherited policy
  provenance. Preserve original upstream implementations and tests in place.
  Native lifecycle execution, complete catalog coverage and per-unit package
  provenance remain unfinished.
