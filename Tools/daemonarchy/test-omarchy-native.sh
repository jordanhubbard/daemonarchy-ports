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
phase()
{
    name=$1
    shift
    code=0
    "$@" >"$out/$name.stdout" 2>"$out/$name.stderr" || code=$?
    printf '%s\n' "$code" >"$out/$name.exit"
    if [ "$code" -ne 0 ]; then
        cat "$out/$name.stdout" "$out/$name.stderr"
        return "$code"
    fi
}
printf '%s\n' "${GITHUB_SHA:-unknown}" >"$out/source-commit.txt"
find "$port" Mk -type f -exec sha256 {} + >"$out/source-inputs.sha256"
pkg query '%n-%v %q' >"$out/bootstrap-before.txt"
phase dependencies make -C "$port" install-missing-packages
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
phase architecture-x86 /usr/local/bin/omarchy-hw-x86
phase platform /usr/local/bin/omarchy-hw-platform
test "$(cat "$out/platform.stdout")" = x86
pkg query '%n-%v %q' >"$out/bootstrap-after.txt"
printf '%s\n' 'Selected FreeBSD port package/install checks only. Full CURRENT closure, native GUI, complete Component lifecycle and physical hardware remain unqualified.' >"$out/scope.txt"
