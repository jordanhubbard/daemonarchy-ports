# Ports development documentation

The root `PROJECT.md` owns requirements and completeness. Original ports and
their native tests remain implementation authority.

- [Initial origin and shared framework](user/ports-units.md)
- [Authority and acceptance map](architecture/design-traceability.md)
- [Active native work](roadmap/active-work.md)
- [Ninja upstream test setup and native observations](verification/ninja-unit-tests-20261124.json)

Use explicit `--repo jordanhubbard/daemonarchy-ports` for GitHub commands; the
fork's upstream repository must not receive downstream-only reviews. The owned
integration branch is `main`, matching the product's release checkout catalog
and HACKING.md. Track FreeBSD upstream `main` separately from the owned branch.
GitHub issues are disabled in this fork;
the product repository's DEV-001 and COMP-001 queues retain the parent work.

- [GDBM supplied-descriptor close-on-exec repair](verification/gdbm-cloexec-native-20261126.json)

- [PBR metadata compatibility observations](verification/pbr-importlib-metadata-native-20261126.json)

- [Linecache2 native consumer observations](verification/linecache2-native-consumers-20261126.json)

- [LZO2 original self-tests and pkg-config header failure](verification/lzo2-pkgconf-native-findings-20261126.json)

- [Pixman image-fill and loader-hook repair, native qualification](verification/pixman-fill-qualified-20261126.json)
