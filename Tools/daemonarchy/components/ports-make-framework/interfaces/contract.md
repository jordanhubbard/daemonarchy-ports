# Shared native ports framework

Provide exact selected original support bytes under `Mk/`, `Templates/`,
`Keywords/` and any additionally observed shared paths. Preserve native metadata
evaluation and fetch/checksum/extract/patch/configure/build/stage/test/package
semantics for the selected target, ABI and options. Evaluation may execute
trusted parse-time commands and therefore needs explicit native authority.

Original ports remain at their upstream paths. Objects, distfiles, options,
staging, package outputs and logs use the explicit parallel-workspace roots.
Never install a build dependency onto the live worker as an implicit action.
Unknown bootstrap provenance, unresolved native edges or missing ownership
remain unqualified; a directory label or source package cannot supply them.

Consumers bind this shared owner's exact revision. A change must invalidate
affected plans, actions, test state and packages. The support unit produces a
source/build pkg with complete selected inputs and provenance; it does not claim
that FreeBSD provides a corresponding independently installable runtime package.
