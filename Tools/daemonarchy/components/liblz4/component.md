---
namespace: daemonarchy-ports
version: 1.0.0
display_name: "Original archivers/liblz4 port"
profiles: ["retained-contract"]
sample: false
provides:
  - name: daemonarchy.ports.liblz4
    version: 1.0.0
    interface:
      uri: interfaces/contract.md
      pin: null
requires: 
  - requirement_id: ports-framework
    capability: daemonarchy.ports.make-framework
    version_range: ">=1,<2"
    dependency_kind: build
    optional: false
    constraints: []
authoring_inputs:
  - kind: specification-to-source-skill
    uri: Tools/daemonarchy/skills/specification-to-source/original-boundaries/SKILL.md
workflow_definition:
  uri: Tools/daemonarchy/workflows/original.md
routing_policy:
  uri: Tools/daemonarchy/routing/original.json
flavor_slots:
  - slot_id: os
    axis: platform.os
    cardinality: exactly-one
    capability_contract: daemonarchy.ports.liblz4
  - slot_id: package
    axis: packaging
    cardinality: exactly-one
    capability_contract: daemonarchy.ports.liblz4
entrypoints: []
acceptance_contracts:
  - kind: acceptance-contract
    uri: acceptance.md
source_dependencies: []
---
# Original archivers/liblz4 port

Preserve the original archivers/liblz4 port, patches, distinfo and upstream implementation. Fetch only checksum-verified selected distfiles and build the library and command-line products with the selected native make configuration. Existing source remains authoritative; do not substitute the installed host LZ4.

Bind real build requirements from native metadata, including gmake and pkgconf versions, rather than assuming that empty runtime LIB_DEPENDS means no build dependencies. Installed bootstrap inputs retain their actual provenance and cannot be relabeled as products of this project.

Run the complete original test inventory for the selected TEST variant against newly built products, then independent acceptance against sealed library/CLI/package bytes. Default check and optional extensive test variants remain distinct. Native ABI, options, shared Mk, patches, source or toolchain changes must invalidate affected objects, test cookies and packages. Produce actual versioned source/build/runtime pkg products with both BOMs and observed typed dependencies.
