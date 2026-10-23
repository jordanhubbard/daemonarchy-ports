---
namespace: daemonarchy-ports
version: 1.0.0
display_name: "Original shared ports make framework"
profiles: ["retained-contract"]
sample: false
provides:
  - name: daemonarchy.ports.make-framework
    version: 1.0.0
    interface:
      uri: interfaces/contract.md
      pin: null
requires: []
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
    capability_contract: daemonarchy.ports.make-framework
  - slot_id: package
    axis: packaging
    cardinality: exactly-one
    capability_contract: daemonarchy.ports.make-framework
entrypoints: []
acceptance_contracts:
  - kind: acceptance-contract
    uri: acceptance.md
source_dependencies: []
---
# Original shared ports make framework

Own selected original Mk, Templates, Keywords and shared-support inputs once. Preserve their native dependency/options/checksum/staging/test/package semantics. Consumers bind exact shared revisions instead of copying authority. This is a source/build support unit, not a newly invented runtime package or complete ports catalog.

Resolve native target, ABI, options, distfiles, patch state and installed bootstrap requirements explicitly. Selected changes must invalidate consumers, original test state and packages. Preserve unprivileged isolated staging and native package content. Missing ownership, unsupported configuration or ambient dependency installation must refuse qualified execution.
