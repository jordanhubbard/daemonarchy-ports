#!/bin/sh
# Evaluate port metadata per architecture without building anything.
# usage (on FreeBSD, PORTSDIR=<tree>): check-arch-deps.sh BASE_TREE HEAD_TREE
# Each tree holds x11-wm/{omarchy,daemonarchy,hyprland}; Mk/ comes from PORTSDIR.
# amd64 metadata must be identical between the trees apart from the additions declared in
# arch-deps-additions; aarch64 must drop the
# dependencies that do not exist there (mise, edk2@bhyve, chromium, aether) and build Hyprland without PCH.
set -eu
base=$1 head=$2
ports='x11-wm/omarchy x11-wm/daemonarchy x11-wm/hyprland'
vars='RUN_DEPENDS BUILD_DEPENDS LIB_DEPENDS CMAKE_ARGS IGNORE'
status=0
here=$(cd "$(dirname "$0")" && pwd)
scratch=$(mktemp -d)
trap 'rm -rf "$scratch"' EXIT
fail() { echo "FAIL: $*" >&2; status=1; }
declared() { # port var entry: is this addition listed in arch-deps-additions?
    grep -qxF -- "$1 $2 $3" "$here/arch-deps-additions"
}
meta() { # tree port arch var
    make -C "$1/$2" ARCH="$3" -V "$4" | tr ' ' '\n' | sed '/^$/d' | sort
}
for port in $ports; do
    for var in $vars; do
        meta "$base" "$port" amd64 "$var" >"$scratch/base"
        meta "$head" "$port" amd64 "$var" >"$scratch/head"
        cmp -s "$scratch/base" "$scratch/head" && continue
        # It differs: only declared additions are allowed, and nothing may disappear.
        [ -z "$(comm -23 "$scratch/base" "$scratch/head")" ] ||
            fail "$port amd64 $var lost an entry the base revision had"
        comm -13 "$scratch/base" "$scratch/head" >"$scratch/added"
        while IFS= read -r entry; do
            declared "$port" "$var" "$entry" ||
                fail "$port amd64 $var differs from the base revision: added '$entry', which is not declared in arch-deps-additions"
        done <"$scratch/added"
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
meta "$head" x11-wm/daemonarchy amd64 RUN_DEPENDS | grep -q ':x11-themes/aether$' || fail 'daemonarchy amd64 lost aether'
# The graphical package manager is wanted on both architectures: its own dependencies (PyGObject, GTK 4) exist on both.
meta "$head" x11-wm/daemonarchy amd64 RUN_DEPENDS | grep -q ':ports-mgmt/daemonarchy-pkg$' || fail 'daemonarchy amd64 lost daemonarchy-pkg'
meta "$head" x11-wm/daemonarchy aarch64 RUN_DEPENDS | grep -q ':ports-mgmt/daemonarchy-pkg$' || fail 'daemonarchy aarch64 lost daemonarchy-pkg'
! meta "$head" x11-wm/daemonarchy aarch64 RUN_DEPENDS | grep -q ':x11-themes/aether$' || fail 'daemonarchy aarch64 still needs aether'
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
[ "$status" = 0 ] && echo 'PASS: amd64 metadata unchanged except the declared additions; aarch64 drops mise, edk2@bhyve, chromium and aether, and disables Hyprland PCH'
exit "$status"
