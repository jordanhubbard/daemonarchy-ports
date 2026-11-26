# Ports development documentation

The root `PROJECT.md` owns requirements and completeness. Original ports and
their native tests remain implementation authority.

- [Initial origin and shared framework](user/ports-units.md)
- [Authority and acceptance map](architecture/design-traceability.md)
- [Active native work](roadmap/active-work.md)
- [Ninja upstream test setup and native observations](verification/ninja-unit-tests-20261124.json)
- [Native Python bootstrap package observations](verification/native-python-bootstrap-20261124.json)

Use explicit `--repo jordanhubbard/daemonarchy-ports` for GitHub commands; the
fork's upstream repository must not receive downstream-only reviews. The owned
integration branch is `main`, matching the product's release checkout catalog
and HACKING.md. Track FreeBSD upstream `main` separately from the owned branch.
GitHub issues are disabled in this fork;
the product repository's DEV-001 and COMP-001 queues retain the parent work.

## Native verification records

- [Brotli original default variant](verification/brotli-original-default-20261126.json)
- [Fontconfig pre-repair default construction](verification/fontconfig-fresh-default-20261126.json)
- [Fontconfig TEST json-c prerequisites](verification/fontconfig-json-c-TEST-20261126.json)
- [Fontconfig JSON configuration test coverage](verification/fontconfig-json-c-test-coverage-20261126.json)
- [Fontconfig original nested-trigger failure and repair](verification/fontconfig-nested-trigger-repair-20261126.json)
- [Pixman fill and loader-hook repair qualification](verification/pixman-fill-qualified-20261126.json)
