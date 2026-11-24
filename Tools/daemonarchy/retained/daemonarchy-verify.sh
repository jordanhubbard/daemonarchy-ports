#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause
# Deterministic acceptance verifier for the retained ports build, exported by the build.
#
#   daemonarchy-verify BUILD_DIR     (the sealed build export, i.e. {output_root}/build)
#
# run-bulk.sh installs this file into the build export (regular file, mode 0755), so the
# Literate AI acceptance policy can run it as an independent case against the sealed
# outputs.  It checks the built package set against the PINNED expectation below and treats
# error TEXT as failure, not just exit status: any text a tool it runs writes to stderr, and
# any error pattern in the captured poudriere logs, is reported on stderr and fails the run.
# On success it prints exactly one fixed line on stdout and nothing on stderr, so the policy
# can pin both streams byte for byte.  It never writes into the export and never installs a
# package.  The pinned values change only with a reviewed change to this file.
set -u
PATH=/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin
export PATH LC_ALL=C

ORIGIN=archivers/liblz4
PKGFILE='liblz4-1.10.0_2,1.pkg'   # PKGNAME of the pinned ports revision (make -V PKGNAME)
JAILTREE=16r2303b36-retained      # poudriere names the package and log directories JAIL-PTREE
# Error patterns searched for in every captured log (extended regular expressions).
PATTERNS='(^|[^[:alnum:]])(Error|ERROR|error):|Error code|FAILED|Segmentation fault|Stop\.$|\*\*\* \[|^pkg: |: undefined reference|cc: error|ld: error'

build=${1:-}
[ -n "$build" ] && [ -d "$build" ] || { echo 'daemonarchy-verify: BUILD_DIR required' >&2; exit 2; }
data=$build/poudriere-data
all=$data/packages/$JAILTREE/All
problems=0
fail() { printf 'daemonarchy-verify %s: %s\n' "$ORIGIN" "$*" >&2; problems=$((problems + 1)); }

# 1. The pinned package exists and is the only package for the origin.
[ -f "$all/$PKGFILE" ] || fail "pinned package $PKGFILE missing"
pkg=$all/$PKGFILE
if [ -f "$pkg" ]; then
	[ -s "$pkg" ] || fail "$PKGFILE is empty"
	out=$(pkg query -F "$pkg" '%o' 2>&1 >/dev/null)
	[ -z "$out" ] || fail "pkg query wrote to stderr: $out"
	origin=$(pkg query -F "$pkg" '%o' 2>/dev/null)
	[ "$origin" = "$ORIGIN" ] || fail "package origin is '$origin', expected $ORIGIN"
	out=$(bsdtar -tf "$pkg" 2>&1 >/dev/null)
	[ -z "$out" ] || fail "bsdtar wrote to stderr: $out"
	bsdtar -tf "$pkg" 2>/dev/null | grep -q '^+MANIFEST$' || fail "$PKGFILE has no +MANIFEST"
	for dep in $(pkg query -F "$pkg" '%dn-%dv' 2>/dev/null); do
		[ -f "$all/$dep.pkg" ] || fail "dependency package $dep.pkg missing"
	done
fi
count=$(ls "$all" 2>/dev/null | grep -c "^liblz4-")
[ "$count" = 1 ] || fail "$count packages named liblz4-* in All, expected exactly 1"

# 2. poudriere recorded no failed, skipped or ignored package, and no error log.
logs=$data/logs
[ -d "$logs" ] || fail "no poudriere logs in the export"
for f in $(find "$logs" -type f \( -name '.poudriere.ports.failed' -o -name '.poudriere.ports.skipped' \
    -o -name '.poudriere.ports.ignored' \) 2>/dev/null); do
	[ ! -s "$f" ] || fail "poudriere recorded entries in $(basename "$f")"
done
if [ -d "$logs" ]; then
	errdir=$(find "$logs" -type d -name errors 2>/dev/null)
	for d in $errdir; do
		[ -z "$(ls -A "$d" 2>/dev/null)" ] || fail "poudriere error logs present: $(ls "$d" | tr '\n' ' ')"
	done
	# 3. Error text in any captured log fails the run, whatever the build's exit status was.
	hits=$(find "$logs" -type f -name '*.log' -exec grep -En "$PATTERNS" {} + 2>/dev/null | head -n 20)
	[ -z "$hits" ] || fail "error text in build logs: $hits"
fi

if [ "$problems" -ne 0 ]; then
	printf 'daemonarchy-verify %s: FAILED problems=%d\n' "$ORIGIN" "$problems" >&2
	exit 1
fi
printf 'daemonarchy-verify %s OK errors=0\n' "$ORIGIN"
