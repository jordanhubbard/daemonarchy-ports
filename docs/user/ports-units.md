# Initial original port closure

The first two canonical contracts select a real origin and its shared native
framework. They do not enumerate every port or establish admission. Original
files remain in place, including unselected origins and support files.

| Unit | Original inputs and relationships |
| --- | --- |
| liblz4 | `archivers/liblz4` port metadata, distinfo and selected patch files; fetched upstream source and original tests are separate checksum-bound inputs. Build dependency on the shared ports framework. |
| ports-make-framework | Exact selected `Mk`, template, keyword and shared-support closure, owned once and referenced by consumers. Registration alone does not cover all support inputs. |

The native make probe selected `gmake>=4.4.1` and `pkgconf>=1.3.0_1` as build
requirements. The eventual profile must bind their actual installed/bootstrap
products or newly admitted producer Components, including runtime libraries.
It may not use empty port-level runtime dependency fields to hide them. Capture
the compiler, sysroot, loader, shell, make and report-producing interpreter too.
Unknown source-build provenance stays explicit.

The port uses the upstream `check` target by default. Enabling TEST selects its
extensive target and Python test dependency; resolve and inventory that exact
variant rather than silently running only the default suite. The direct native
probe ran default checks against the newly built programs and created an
ordinary native pkg. Original inventory/report binding, independent sealed
acceptance and provenance admission through LitAI remain open.

Distfile/patch, source, shared Mk, options, ABI, dependencies or compiler changes
must invalidate the correct closure, including ports completion cookies.
Packaging, build directories and ABI/toolchain choices belong in the selected
Flavors and reviewed retained profile. Do not execute a whole-tree wrapper and
call it per-origin Component acceptance.
