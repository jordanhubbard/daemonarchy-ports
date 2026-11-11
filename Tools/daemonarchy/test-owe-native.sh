#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause
# Original owe port checks across native inotify and its retained compatibility path.
set -eu
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
freebsd-version -ku >"$out/platform.txt"
git rev-parse HEAD >"$out/source-commit.txt"
sha256 "$port/Makefile" "$port/distinfo" >"$out/source-inputs.sha256"
phase options make -C "$port" -V LIB_DEPENDS -V USES -V LDFLAGS
if test -f /usr/include/sys/inotify.h; then
    echo system >"$out/inotify-provider.txt"
    ! grep -q 'libinotify.so:devel/libinotify' "$out/options.stdout"
else
    echo compatibility >"$out/inotify-provider.txt"
    grep -q 'libinotify.so:devel/libinotify' "$out/options.stdout"
fi
phase dependencies make -C "$port" install-missing-packages
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
phase linked-libraries ldd /usr/local/bin/owe
! grep -q 'not found' "$out/linked-libraries.stdout"
if test "$(cat "$out/inotify-provider.txt")" = compatibility; then
    grep -q libinotify "$out/linked-libraries.stdout"
fi
phase deinstall pkg delete -y owe
! pkg info -e owe
printf '%s\n' 'Original native port package/install/checksum/link/deinstall checks. No GUI, final CURRENT closure or SDK/lifecycle admission.' >"$out/scope.txt"
