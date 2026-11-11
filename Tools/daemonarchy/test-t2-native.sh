#!/bin/sh
# Disposable native FreeBSD port build; never run on the operator workstation.
# This records commands and real outputs, not Component or release admission.
set -eu
test "$(uname -s)" = FreeBSD
test "$(uname -m)" = amd64
test "$(id -u)" = 0
root=$(pwd -P)
out="$root/_build/t2-native"
test ! -e "$out"
mkdir -p "$out"
freebsd-version -ku >"$out/freebsd-version.txt"
sysctl hw.machine_arch hw.ncpu hw.physmem kern.osreldate >"$out/platform.txt"
df -k "$out" >"$out/storage.txt"
available=$(df -k "$out" | awk 'NR == 2 { print $4 }')
# Selected two-port validation needs 8 GiB plus 2 GiB reserve. This is not
# the separate full-world/SDK worker profile and grants no such admission.
test "$available" -ge 10485760
printf '%s\n' "${GITHUB_SHA:-unknown}" >"$out/source-commit.txt"
find security/t2-touchid security/libfprint devel/libgusb Mk -type f -exec sha256 {} + >"$out/source-inputs.sha256"
pkg query '%n-%v %q' >"$out/bootstrap-before.txt"
export PORTSDIR="$root"
export BATCH=yes
export MAKE_JOBS_NUMBER=2
export CARGO_BUILD_JOBS=2
ulimit -c 0
phase()
{
    name=$1
    shift
    result=0
    printf 'phase %s\n' "$name"
    "$@" >"$out/$name.stdout" 2>"$out/$name.stderr" || result=$?
    printf '%s\n' "$result" >"$out/$name.exit"
    if [ "$result" -ne 0 ]; then
        cat "$out/$name.stdout" "$out/$name.stderr"
        return "$result"
    fi
}
phase libfprint-dependencies make -C security/libfprint install-missing-packages
phase glib-control-compile sh -c 'cc -g -Wall -Wextra -Werror $(pkg-config --cflags gio-2.0 gusb) Tools/daemonarchy/test-glib-native.c $(pkg-config --libs gio-2.0 gusb) -o "$1/glib-control"' sh "$out"
pkg query '%n-%v %q' libgusb >"$out/libgusb-original-version.txt"
# Preserve the old dependency observation separately from required fixed tests.
phase gusb-original-dispose "$out/glib-control" --usb-unenumerated-dispose || :
phase libgusb-dependencies make -C devel/libgusb install-missing-packages
phase libgusb-build make -C devel/libgusb package
failures=0
phase libgusb-tests make -C devel/libgusb test || failures=1
libgusb_build=$(make -C devel/libgusb -V BUILD_WRKSRC)
mkdir -p "$out/libgusb-meson-logs"
cp "$libgusb_build"/meson-logs/testlog* "$out/libgusb-meson-logs/"
phase libgusb-install make -C devel/libgusb reinstall
phase glib-control "$out/glib-control" || failures=1
phase gusb-control "$out/glib-control" --usb || failures=1
phase gusb-dispose-control "$out/glib-control" --usb-dispose || failures=1
phase gusb-unenumerated-dispose-control "$out/glib-control" --usb-unenumerated-dispose || failures=1
phase libfprint-checksum make -C security/libfprint checksum
phase libfprint-build make -C security/libfprint package
phase libfprint-tests make -C security/libfprint test || failures=1
libfprint_build=$(make -C security/libfprint -V BUILD_WRKSRC)
mkdir -p "$out/libfprint-meson-logs"
cp "$libfprint_build"/meson-logs/testlog* "$out/libfprint-meson-logs/"
if [ "$failures" -ne 0 ]; then
    phase debugger-dependencies pkg install -y gdb python312
    phase libfprint-backtraces /usr/local/bin/python3.12 Tools/daemonarchy/diagnose-libfprint.py "$libfprint_build" "$out/libfprint-debug"
    sanitized="$root/_build/libfprint-sanitized"
    sanitized_args="$(make -C security/libfprint -V MESON_ARGS) -Db_sanitize=address,undefined -Db_lundef=false -Ddoc=false"
    # The diagnostic profile disables only documentation: make test stages the
    # library first, and gtk-doc assumes GCC -lasan/-lubsan names on this host.
    # All original production options, package and tests above remain required.
    if phase libfprint-sanitized-build make -C security/libfprint WRKDIRPREFIX="$sanitized" MESON_ARGS="$sanitized_args" build; then
        phase libfprint-sanitized-tests make -C security/libfprint WRKDIRPREFIX="$sanitized" MESON_ARGS="$sanitized_args" test || failures=1
        sanitized_build=$(make -C security/libfprint WRKDIRPREFIX="$sanitized" -V BUILD_WRKSRC)
        mkdir -p "$out/libfprint-sanitized-meson-logs"
        cp "$sanitized_build"/meson-logs/testlog* "$out/libfprint-sanitized-meson-logs/"
    fi
    baseline="$root/_build/libfprint-default"
    phase libfprint-baseline-build make -C security/libfprint WRKDIRPREFIX="$baseline" OPTIONS_UNSET=VIRTUAL package
    phase libfprint-baseline-tests make -C security/libfprint WRKDIRPREFIX="$baseline" OPTIONS_UNSET=VIRTUAL test || failures=1
    baseline_build=$(make -C security/libfprint WRKDIRPREFIX="$baseline" OPTIONS_UNSET=VIRTUAL -V BUILD_WRKSRC)
    mkdir -p "$out/libfprint-baseline-meson-logs"
    cp "$baseline_build"/meson-logs/testlog* "$out/libfprint-baseline-meson-logs/"
fi
phase libfprint-install make -C security/libfprint install
phase touchid-dependencies make -C security/t2-touchid install-missing-packages
phase touchid-checksum make -C security/t2-touchid checksum
phase touchid-build make -C security/t2-touchid package
phase touchid-tests make -C security/t2-touchid test
phase touchid-install make -C security/t2-touchid install
cli_refusal()
{
    code=0
    timeout 5 /usr/local/bin/t2-touchid --ci-unknown-option >"$out/cli.stdout" 2>"$out/cli.stderr" || code=$?
    printf '%s\n' "$code" >"$out/cli.exit"
    test "$code" -eq 1
    grep -Fq 'unknown argument --ci-unknown-option' "$out/cli.stderr"
}
phase touchid-cli-refusal cli_refusal
pkg query '%n-%v %q' >"$out/bootstrap-after.txt"
pkg info -f libgusb libfprint t2-touchid >"$out/packages.txt"
pkg info -l libgusb libfprint t2-touchid >"$out/installed-files.txt"
sha256 /usr/local/bin/t2-touchid /usr/local/lib/libfprint-2.so.2 /usr/local/lib/libgusb.so.2 >"$out/installed.sha256"
# Inspect exactly the installed service syntax without starting hardware I/O.
phase touchid-service-syntax sh -n /usr/local/etc/rc.d/t2_touchid
printf '%s\n' "$failures" >"$out/required-test-failures"
printf '%s\n' 'Inspect every recorded original test count and failure. No physical T2, CURRENT closure, SDK or lifecycle qualification is awarded by these commands.' >"$out/scope.txt"
test "$failures" -eq 0
