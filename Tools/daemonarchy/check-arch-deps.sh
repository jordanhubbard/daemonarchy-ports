#!/bin/sh
# Evaluate port metadata per architecture without building anything.
# usage (on FreeBSD, PORTSDIR=<tree>): check-arch-deps.sh BASE_TREE HEAD_TREE
# Each tree holds x11-wm/{omarchy,daemonarchy,hyprland}; Mk/ comes from PORTSDIR.
# amd64 metadata must be identical between the trees; aarch64 must drop the
# dependencies that do not exist there (mise, edk2@bhyve, chromium) and build Hyprland without PCH.
set -eu
base=$1 head=$2
ports='x11-wm/omarchy x11-wm/daemonarchy x11-wm/hyprland'
vars='RUN_DEPENDS BUILD_DEPENDS LIB_DEPENDS CMAKE_ARGS IGNORE'
status=0
fail() { echo "FAIL: $*" >&2; status=1; }
meta() { # tree port arch var
    make -C "$1/$2" ARCH="$3" -V "$4" | tr ' ' '\n' | sed '/^$/d' | sort
}
for port in $ports; do
    for var in $vars; do
        [ "$(meta "$base" "$port" amd64 "$var")" = "$(meta "$head" "$port" amd64 "$var")" ] ||
            fail "$port amd64 $var differs from the base revision"
    done
    [ -z "$(meta "$head" "$port" aarch64 IGNORE)" ] || fail "$port is ignored on aarch64"
done
for port in x11-wm/omarchy x11-wm/daemonarchy; do
    meta "$head" "$port" amd64 RUN_DEPENDS | grep -q ':sysutils/mise$' || fail "$port amd64 lost mise"
    ! meta "$head" "$port" aarch64 RUN_DEPENDS | grep -q ':sysutils/mise$' || fail "$port aarch64 still needs mise"
done
meta "$head" x11-wm/daemonarchy amd64 RUN_DEPENDS | grep -q ':sysutils/edk2@bhyve$' || fail 'daemonarchy amd64 lost edk2@bhyve'
! meta "$head" x11-wm/daemonarchy aarch64 RUN_DEPENDS | grep -q 'sysutils/edk2' || fail 'daemonarchy aarch64 still needs edk2@bhyve'
meta "$head" x11-wm/daemonarchy amd64 RUN_DEPENDS | grep -q ':www/chromium$' || fail 'daemonarchy amd64 lost chromium'
! meta "$head" x11-wm/daemonarchy aarch64 RUN_DEPENDS | grep -q ':www/chromium$' || fail 'daemonarchy aarch64 still needs chromium'
meta "$head" x11-wm/hyprland aarch64 CMAKE_ARGS | grep -q 'CMAKE_DISABLE_PRECOMPILE_HEADERS:BOOL=ON' || fail 'hyprland aarch64 keeps PCH'
! meta "$head" x11-wm/hyprland amd64 CMAKE_ARGS | grep -q 'CMAKE_DISABLE_PRECOMPILE_HEADERS' || fail 'hyprland amd64 changed PCH'
for port in $ports; do
    for tree in "$base" "$head"; do
        (cd "$tree/$port" && portlint -C) >"$tree/${port##*/}.portlint" 2>&1 || true
    done
    before=$(grep -c '^FATAL' "$base/${port##*/}.portlint" || true)
    after=$(grep -c '^FATAL' "$head/${port##*/}.portlint" || true)
    echo "== portlint $port: FATAL $before -> $after"
    grep -E '^(FATAL|WARN)' "$head/${port##*/}.portlint" || true
    [ "$after" -le "$before" ] || fail "portlint reports new FATAL errors for $port"
done
[ "$status" = 0 ] && echo 'PASS: amd64 metadata unchanged; aarch64 drops mise, edk2@bhyve and chromium, and disables Hyprland PCH'
exit "$status"
