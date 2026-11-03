#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause
# DRAFT retained-run wrapper for `poudriere bulk`: jail/tree names and output mapping.
#
#   sh Tools/daemonarchy/retained/run-bulk.sh OUTDIR
#
# The retained profile cannot pass environment variables or create a ports-tree registration,
# so this wrapper (a) copies the reviewed poudriere registry into the run's private TMPDIR so
# the shared registry is never written, (b) points POUDRIERE_DATA and DISTFILES_CACHE at OUTDIR
# (the profile passes {output_root}/build), so logs, caches and the built package set are the
# sealed output, (c) registers the retained snapshot itself as ports tree PTREE (null method),
# (d) bulk-builds the origins in origins.list with `-b latest -t`, and (e) installs
# daemonarchy-verify.sh into the build export, the executable the acceptance policy runs.
#
# poudriere needs root (jails, mounts); the wrapper refuses otherwise.  Nothing is signed.
set -eu
[ $# -eq 1 ] || { echo 'usage: run-bulk.sh OUTDIR' >&2; exit 2; }
out=$1
here=$(cd "$(dirname "$0")/../../.." && pwd)
[ "$(id -u)" = 0 ] || { echo 'run-bulk.sh: poudriere requires root' >&2; exit 1; }
case "$out" in /*) ;; *) echo 'OUTDIR must be absolute' >&2; exit 2 ;; esac
[ -n "${TMPDIR:-}" ] && [ -d "$TMPDIR" ] || { echo 'the retained run must provide TMPDIR' >&2; exit 2; }
set -a
. "$here/Tools/daemonarchy/retained/bulk.env"
set +a
PATH=$here/closure/toolchain/bin:$here/closure/dependency/bin:/sbin:/bin:/usr/sbin:/usr/bin
export PATH
mkdir -p "$out/poudriere-data" "$out/distfiles"
etc=$TMPDIR/poudriere-etc
cp -R "$POUDRIERE_ETC_SOURCE" "$etc"
sed -i '' -e "s|^POUDRIERE_DATA=.*|POUDRIERE_DATA=$out/poudriere-data|" \
          -e "s|^DISTFILES_CACHE=.*|DISTFILES_CACHE=$out/distfiles|" "$etc/poudriere.conf"
printf '%s\n' 'PACKAGE_FETCH_URL="https://pkg.FreeBSD.org/${ABI}"' >"$etc/poudriere.d/$PTREE-poudriere.conf"
cd "$here"
poudriere -e "$etc" ports -c -p "$PTREE" -m null -M "$here"
poudriere -e "$etc" bulk -j "$JAIL" -p "$PTREE" -b latest -t -f "$here/$ORIGINS"
# Export the deterministic acceptance verifier with the build (the profile's package
# inventory is this directory); the acceptance policy runs it against the sealed exports.
install -m 0755 "$here/Tools/daemonarchy/retained/daemonarchy-verify.sh" "$out/daemonarchy-verify"
