#!/bin/sh
# Selected original port qualification in a disposable FreeBSD guest.
set -eu
test "$(uname -s)" = FreeBSD
test "$(id -u)" = 0
root=$(pwd -P)
out="$root/_build/omarchy-native"
test ! -e "$out"
mkdir -p "$out/packages"
freebsd-version -ku >"$out/platform.txt"
df -k "$out" >"$out/storage.txt"
# Selected desktop port assembly needs 12 GiB plus 4 GiB reserve.
# This is not the separate 90 GiB full-world/SDK admission profile.
test "$(df -k "$out" | awk 'NR == 2 {print $4}')" -ge 16777216
export PORTSDIR="$root"
export BATCH=yes
export MAKE_JOBS_NUMBER=2
port=x11-wm/omarchy
# Retain the complete original source, staging tree and products on both success
# and failure. The workflow uploads this directory after the guest syncs back.
preserve_original_workdir()
{
    original_status=$?
    trap - EXIT
    archive_status=0
    if workdir=$(make -C "$root/$port" -V WRKDIR 2>"$out/preserve-workdir-query.stderr"); then
        if [ -n "$workdir" ] && [ "$workdir" != / ] && [ -d "$workdir" ]; then
            tar --acls --fflags -czf "$out/original-selected-workdir.tar.gz" \
                -C "$(dirname "$workdir")" "$(basename "$workdir")" \
                >"$out/preserve-workdir.stdout" 2>"$out/preserve-workdir.stderr" || archive_status=$?
        else
            printf '%s\n' 'Original selected work directory was not created.' >"$out/preserve-workdir.stdout"
        fi
    else
        archive_status=1
    fi
    printf '%s\n' "$archive_status" >"$out/preserve-workdir.exit"
    if [ "$original_status" -eq 0 ] && [ "$archive_status" -ne 0 ]; then
        original_status=1
    fi
    exit "$original_status"
}
trap preserve_original_workdir EXIT
phase()
{
    name=$1
    shift
    printf "%s\t%s\tstart\n" "$(date -u +%Y-%m-%dT%H:%M:%SZ)" "$name" >>"$out/phases.tsv"
    code=0
    "$@" >"$out/$name.stdout" 2>"$out/$name.stderr" || code=$?
    printf '%s\n' "$code" >"$out/$name.exit"
    printf "%s\t%s\texit=%s\n" "$(date -u +%Y-%m-%dT%H:%M:%SZ)" "$name" "$code" >>"$out/phases.tsv"
    if [ "$code" -ne 0 ]; then
        cat "$out/$name.stdout" "$out/$name.stderr"
        return "$code"
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
printf '%s\n' "${GITHUB_SHA:-unknown}" >"$out/source-commit.txt"
find "$port" multimedia/libdvdcss Mk -type f -exec sha256 {} + >"$out/source-inputs.sha256"
pkg query '%n-%v %q' >"$out/bootstrap-before.txt"
# The official binary repository cannot distribute libdvdcss. Build this
# actual selected dependency from its checksum-bound original port first.
phase restricted-bootstrap pkg install -y meson pkgconf
# DOCS is a native default. Resolve its declared doxygen/tool dependencies
# through the ordinary binary bootstrap path before invoking original make;
# do not accidentally rebuild the transitive documentation toolchain here.
phase libdvdcss-options make -C multimedia/libdvdcss -V PORT_OPTIONS
# Fetch direct prerequisites as binaries, then enforce native constraints.
phase libdvdcss-dependencies bootstrap_dependencies multimedia/libdvdcss libdvdcss
phase libdvdcss-checksum make -C multimedia/libdvdcss checksum
phase libdvdcss-package make -C multimedia/libdvdcss package
dependency_package=$(make -C multimedia/libdvdcss -V WRKDIR_PKGFILE)
test -f "$dependency_package"
cp -p "$dependency_package" "$out/packages/"
phase libdvdcss-install make -C multimedia/libdvdcss install
# Retain the original recursive expression for audit. Native dependency
# targets below validate the selected prerequisites and log their resolution.
phase dependency-expression make -C "$port" -V MISSING-DEPENDS-LIST
phase dependencies bootstrap_dependencies "$port" selected-port
phase checksum make -C "$port" checksum
result=0
phase package make -C "$port" DEVELOPER=yes package || result=$?
# Retain the native plist even when staging exposes a packaging discrepancy.
phase generated-plist make -C "$port" makeplist || result=1
phase check-plist make -C "$port" check-plist || result=1
test "$result" -eq 0
package=$(make -C "$port" -V WRKDIR_PKGFILE)
test -f "$package"
cp -p "$package" "$out/packages/"
sha256 "$out"/packages/*.pkg >"$out/package-archives.sha256"
phase install make -C "$port" install
pkg info -f omarchy >"$out/package-info.txt"
pkg info -l omarchy >"$out/installed-files.txt"
phase package-checksums pkg check -s omarchy
# Exercise the installed command as an unprivileged user. A successful package
# checksum check alone cannot find runtime assets omitted from the packing list.
icons_python=$(make -C "$port" -V PYTHON_CMD)
icons_prefix=$(make -C "$port" -V PREFIX)
icons_datadir=$(make -C "$port" -V DATADIR)
phase built-in-icon-sets "$icons_python" - "$icons_prefix" "$icons_datadir" <<'PYICONS'
import os
from pathlib import Path
import pwd
import shutil
import subprocess
import sys
import tempfile

account = pwd.getpwnam("nobody")
fixture = tempfile.mkdtemp(prefix="omarchy-icons-")
try:
    os.chown(fixture, account.pw_uid, account.pw_gid)
    context = {
        "env": {
            "HOME": fixture,
            "OMARCHY_PATH": sys.argv[2],
            "PATH": f"{sys.argv[1]}/bin:/usr/bin:/bin",
            "LANG": "C",
        },
        "user": account.pw_uid,
        "group": account.pw_gid,
        "extra_groups": [],
        "capture_output": True,
        "text": True,
    }

    def checked_output(command):
        result = subprocess.run(command, **context)
        print(result.stdout, end="")
        print(result.stderr, end="", file=sys.stderr)
        result.check_returncode()
        return result.stdout

    uid = checked_output(["/usr/bin/id", "-u"]).strip()
    assert int(uid) == account.pw_uid and int(uid) != 0, uid
    command = str(Path(sys.argv[1]) / "bin/omarchy-icons-list")
    labels = checked_output([command]).splitlines()
    assert {"Arcade", "Papercraft", "Original"}.issubset(labels), labels
finally:
    shutil.rmtree(fixture)
PYICONS
# Measure the actual installed privileged files, including numeric ownership.
installed_ownership()
{
    for installed_file in \
        "$icons_prefix/bin/omarchy-firmware-setup" \
        "$icons_datadir/freebsd/bin/omarchy-firmware-setup" \
        "$icons_prefix/libexec/omarchy/compat/systemctl" \
        "$icons_prefix/etc/polkit-1/rules.d/50-omarchy.rules"
    do
        metadata=$(stat -f '%u:%g:%Lp' "$installed_file") || return $?
        printf '%s %s\n' "$metadata" "$installed_file" || return $?
        case "$metadata" in
            0:0:*) permissions=${metadata#0:0:} ;;
            *) return 1 ;;
        esac
        test "$((0$permissions & 06022))" -eq 0 || return $?
    done
}
phase installed-ownership installed_ownership
phase desktop-version env -u OMARCHY_PATH /usr/local/bin/omarchy-version
expected_version=$(make -C "$port" -V PKGVERSION)
test "$(cat "$out/desktop-version.stdout")" = "$expected_version"
phase architecture-x86 /usr/local/bin/omarchy-hw-x86
phase platform /usr/local/bin/omarchy-hw-platform
test "$(cat "$out/platform.stdout")" = x86
pkg query '%n-%v %q' >"$out/bootstrap-after.txt"
printf '%s\n' 'Selected FreeBSD port package/install checks only. Full CURRENT closure, native GUI, complete Component lifecycle and physical hardware remain unqualified.' >"$out/scope.txt"
