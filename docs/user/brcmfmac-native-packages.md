# Broadcom native module package qualification

The `net/if_brcmfmac-kmod` revision 4 candidate adds aarch64 while retaining
amd64 and all nine existing downstream patches. The original archive remains
pinned to `9adbe774d3be91827d628fbd811daf63512acb6d`. Patches apply with zero
fuzz. Native FreeBSD CTF generation and a CTF-preserving final strip retain
driver and net80211 type information without shipping DWARF in the module.

For custom kernels, pass `SRC_BASE` and `KERNBUILDDIR` together. The latter
selects the completed kernel's generated option headers. This qualification
used the F4 source snapshot, an arm64 GENERIC-MMCCAM kernel with three mailbox
overlays, and a matching amd64 GENERIC kernel. These are development artifacts;
they do not establish the final matching CURRENT release closure.

The [dated record](../../verification/brcmfmac-native-packages-20261122.json)
binds both actual packages, kernels, unchanged patches, native phases and
installed payloads. Both native workers completed the original checksum,
patch, build, stage, plist, stage-QA and package targets. Each retained 406
selected ports inputs, 36,893 kernel source entries and all 55 patched source
files with before/after guards. The arm64 custom-kernel capsule additionally
retained 307 generated-header, overlay and kernel entries. Neither upstream
nor this port declares an executable test suite: the successful ports `test`
target is not an upstream test-suite result.

Independent disposable FreeBSD consumers installed the actual packages with
their original scripts, checked all five payload files' bytes, modes and root
ownership, package checksums, registry, linker hints, licenses and generated
pc-format SBOM. They exercised two installed-driver load/unload cycles across
package deletion and reinstallation. Both PCI and SDIO frontends registered;
neither guest had a Broadcom radio. The prior missing-CTF warning was absent
for these packages. The arm64 consumer restored its original module/interface
state; both test VMs were shut down and their frozen backing images preserved.

On amd64, the base `sdio.ko` dependency remains loaded after the driver unloads:
the original `sys/dev/sdio/sdiob.c` deinitialization returns `EOPNOTSUPP`.
The refused normal unload and first consumer assertion are retained. No forced
unload was used. A subsequent actual reboot of that same disposable disk
restored the original module list and confirmed that the deleted package's
registry entry and all five payload files were absent. This is reboot cleanup
evidence, not support for unloading the base SDIO subsystem.

Original source, complete build work, generated inputs, products, native
logs and failed attempts are retained in checksum-bound archives. Observed
bootstrap tool and runtime-library identities are recorded; complete original
toolchain source provenance and canonical Component/SDK/lifecycle admission
remain open. The framework SBOM is retained without claiming both canonical
release BOMs or a lifecycle receipt.

Before enabling Pi Wi-Fi in an image, qualify ACPI SDIO enumeration, power and
pin sequencing, board-specific firmware/NVRAM/CLM, association, traffic,
recovery, and native SD/Ethernet regression. Physical PCIe radio operation,
product install/update/rollback and final 1.6 release acceptance also remain
open. These packages have not been published to a signed feed.
