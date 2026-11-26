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

- [Fontconfig older cache baseline 20261126](verification/fontconfig-older-cache-baseline-20261126.json)
- [Fontconfig trigger fixed qualified 20261126](verification/fontconfig-trigger-fixed-qualified-20261126.json)
- [Freetype original default 20261126](verification/freetype-original-default-20261126.json)
- [Gperf original default 20261126](verification/gperf-original-default-20261126.json)
- [Json c original suite 20261126](verification/json-c-original-suite-20261126.json)
- [Json c qualified 20261126](verification/json-c-qualified-20261126.json)
- [Libdatrie original default 20261126](verification/libdatrie-original-default-20261126.json)
- [Libthai original default 20261126](verification/libthai-original-default-20261126.json)
- [Libyaml original default 20261126](verification/libyaml-original-default-20261126.json)
- [More itertools original default 20261126](verification/more-itertools-original-default-20261126.json)
- [Pixman native findings 20261126](verification/pixman-native-findings-20261126.json)
- [Png original default 20261126](verification/png-original-default-20261126.json)
- [Pytest tap original suite 20261126](verification/pytest-tap-original-suite-20261126.json)
- [Pytest tap qualified 20261126](verification/pytest-tap-qualified-20261126.json)
- [Pyyaml test import default 20261126](verification/pyyaml-test-import-default-20261126.json)
- [Tap py parser boundaries 20261126](verification/tap-py-parser-boundaries-20261126.json)
- [Xxd original default 20261126](verification/xxd-original-default-20261126.json)

- [LZO2 original self-tests and pkg-config header failure](verification/lzo2-pkgconf-native-findings-20261126.json)

- [GObject Introspection full default native qualification](verification/gobject-introspection-full-default-20261126.json)

- [Mock original native suite and normal qualification](verification/mock-original-native-20261126.json)

- [Nose compatibility and Python grammar permission findings](verification/nose-python312-native-findings-20261126.json)

- [Nose conversion repair control](verification/nose-conversion-native-control-20261126.json)
