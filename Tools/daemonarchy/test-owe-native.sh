#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause
# Original owe port checks across native inotify and its retained compatibility path.
set -eu
revision=${1:?reviewed source revision required}
test "${#revision}" -eq 40
case "$revision" in *[!0-9a-f]*) exit 64;; esac
test "$(uname -s)" = FreeBSD
test "$(id -u)" = 0
root=$(pwd -P)
out="$root/_build/owe-native"
test ! -e "$out"
mkdir -p "$out/packages"
test "$(df -k "$out" | awk 'NR == 2 {print $4}')" -ge 16777216
export PORTSDIR="$root" BATCH=yes MAKE_JOBS_NUMBER=2
port=x11/owe
phase() {
    name=$1; shift
    printf '%s\t%s\tstart\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ)" "$name" >>"$out/phases.tsv"
    code=0
    "$@" >"$out/$name.stdout" 2>"$out/$name.stderr" || code=$?
    printf '%s\n' "$code" >"$out/$name.exit"
    printf '%s\t%s\texit=%s\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ)" "$name" "$code" >>"$out/phases.tsv"
    if [ "$code" -ne 0 ]; then cat "$out/$name.stdout" "$out/$name.stderr"; return "$code"; fi
}
expect_absent() {
    absence_code=0
    "$@" || absence_code=$?
    if [ "$absence_code" -ne 1 ]; then
        printf 'Expected absence (exit 1), got %s: %s\n' "$absence_code" "$*" >&2
        return 1
    fi
}
bootstrap_dependencies() {
    dependency_root=$1
    dependency_label=$2
    # These native list targets enumerate this port's direct prerequisites.
    # pkg supplies their runtime closure; their source-build tools are unrelated.
    make -C "$dependency_root" DEPENDS_SHOW_FLAVOR=yes \
        build-depends-list run-depends-list test-depends-list \
        >"$out/$dependency_label.direct-paths" || return $?
    sort -u "$out/$dependency_label.direct-paths" \
        >"$out/$dependency_label.direct-paths.sorted" || return $?
    pkg query -a '%o' >"$out/$dependency_label.installed-origins" || return $?
    : >"$out/$dependency_label.binary-origins" || return $?
    while IFS= read -r dependency_path; do
        case "$dependency_path" in
            "$root/"*) dependency_origin=${dependency_path#"$root/"} ;;
            *) printf 'Unexpected dependency path: %s\n' "$dependency_path" >&2; return 1 ;;
        esac
        # Installed origins omit the flavor/subpackage suffix. The original
        # depends targets below still enforce the actual package/file/library
        # constraints, and build a dependency if its installed variant is wrong.
        installed_origin=${dependency_origin%@*}
        installed_origin=${installed_origin%~*}
        if grep -Fqx "$installed_origin" "$out/$dependency_label.installed-origins"; then
            continue
        else
            dependency_status=$?
            test "$dependency_status" -eq 1 || return "$dependency_status"
        fi
        printf '%s\n' "$dependency_origin" >>"$out/$dependency_label.binary-origins" || return $?
    done <"$out/$dependency_label.direct-paths.sorted"
    while IFS= read -r dependency_origin; do
        pkg install -yA "$dependency_origin" || return $?
    done <"$out/$dependency_label.binary-origins"
    make -C "$dependency_root" USE_PACKAGE_DEPENDS=yes depends test-depends
}
freebsd-version -ku >"$out/platform.txt"
printf '%s\n' "$revision" >"$out/source-commit.txt"
find "$port" multimedia/libdvdcss Mk -type f -exec sha256 {} + >"$out/source-inputs.sha256"
phase options make -C "$port" -V LIB_DEPENDS -V USES -V LDFLAGS
if test -f /usr/include/sys/inotify.h; then
    echo system >"$out/inotify-provider.txt"
    expect_absent grep -q 'libinotify.so:devel/libinotify' "$out/options.stdout"
else
    echo compatibility >"$out/inotify-provider.txt"
    grep -q 'libinotify.so:devel/libinotify' "$out/options.stdout"
fi
# The original runtime dependency closure includes non-distributable libdvdcss.
# Build its checksum-bound port with DOCS defaults retained, just as in the
# Omarchy native lane, before the ordinary binary dependency bootstrap.
phase restricted-bootstrap pkg install -y meson pkgconf
phase libdvdcss-options make -C multimedia/libdvdcss -V PORT_OPTIONS
# Fetch direct prerequisites as binaries, then enforce native constraints.
phase libdvdcss-dependencies bootstrap_dependencies multimedia/libdvdcss libdvdcss
phase libdvdcss-checksum make -C multimedia/libdvdcss checksum
phase libdvdcss-package make -C multimedia/libdvdcss package
dependency_package=$(make -C multimedia/libdvdcss -V WRKDIR_PKGFILE)
test -f "$dependency_package"
cp -p "$dependency_package" "$out/packages/"
phase libdvdcss-install make -C multimedia/libdvdcss install
phase dependencies bootstrap_dependencies "$port" selected-port
phase checksum make -C "$port" checksum
phase package make -C "$port" DEVELOPER=yes package
phase check-plist make -C "$port" check-plist
package=$(make -C "$port" -V WRKDIR_PKGFILE)
test -f "$package"
cp -p "$package" "$out/packages/"
sha256 "$out"/packages/*.pkg >"$out/package-archives.sha256"
phase install make -C "$port" install
pkg info -f owe >"$out/package-info.txt"
phase package-checksums pkg check -s owe
phase linked-libraries ldd /usr/local/bin/owe /usr/local/bin/owed /usr/local/bin/owe-render
expect_absent grep -q 'not found' "$out/linked-libraries.stdout"
if test "$(cat "$out/inotify-provider.txt")" = compatibility; then
    # File watching belongs to the daemon; --as-needed correctly omits it
    # from the command-line client. Inspect the consuming executable itself.
    phase daemon-linked-libraries ldd /usr/local/bin/owed
    grep -q libinotify "$out/daemon-linked-libraries.stdout"
fi
phase deinstall pkg delete -y owe
expect_absent pkg info -e owe
printf '%s\n' 'Original native port package/install/checksum/link/deinstall checks. No GUI, final CURRENT closure or SDK/lifecycle admission.' >"$out/scope.txt"
