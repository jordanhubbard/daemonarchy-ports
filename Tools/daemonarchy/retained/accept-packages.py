#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
"""DRAFT: re-observe the package set `poudriere bulk` sealed, separately from poudriere.

For every origin in the origins list: exactly one package in <packages>/All has that origin
(`pkg query -F`), the package is a readable archive carrying +MANIFEST, it is non-empty, and
every dependency it declares is also present in All.  It records each package's sha256.
It never installs a package, runs a port or reads a key.  STATIC package acceptance only:
installation, the port's own test results and runtime behaviour are not examined (UNKNOWN: no
retained action defines them).  Exit status is nonzero when any check fails.

    accept-packages.py --packages DIR --origins FILE --output report.json
"""
import argparse
import hashlib
import json
import subprocess
import sys
from pathlib import Path


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def query(pkgfile, fmt):
    r = subprocess.run(["pkg", "query", "-F", str(pkgfile), fmt], capture_output=True, text=True, timeout=300)
    return r.stdout.strip().splitlines() if r.returncode == 0 else None


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--packages", required=True)
    ap.add_argument("--origins", required=True)
    ap.add_argument("--output", required=True)
    args = ap.parse_args()
    allp = Path(args.packages) / "All"
    origins = [l.strip() for l in Path(args.origins).read_text().splitlines() if l.strip() and not l.startswith("#")]
    pkgs = sorted(p for p in allp.glob("*.pkg")) if allp.is_dir() else []
    names = {p.name for p in pkgs}
    by_origin = {}
    for p in pkgs:
        origin = query(p, "%o")
        if origin:
            by_origin.setdefault(origin[0], []).append(p)
    results = []
    for origin in origins:
        found = by_origin.get(origin, [])
        entry = {"origin": origin, "checks": {}}
        entry["checks"]["exactly-one-package"] = len(found) == 1
        if len(found) == 1:
            p = found[0]
            entry["package"] = p.name
            entry["sha256"] = sha256_file(p)
            entry["checks"]["non-empty"] = p.stat().st_size > 0
            listing = subprocess.run(["bsdtar", "-tf", str(p)], capture_output=True, text=True, timeout=300)
            entry["checks"]["manifest-in-archive"] = listing.returncode == 0 and "+MANIFEST" in listing.stdout
            deps = query(p, "%dn-%dv") or []
            missing = [d for d in deps if not any(n.startswith(d) for n in names)]
            entry["checks"]["dependencies-present"] = not missing
            if missing:
                entry["missing_dependencies"] = missing
        results.append(entry)
    out = Path(args.output)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps({"schema": "daemonarchy/package-acceptance@1", "results": results},
                              indent=1) + "\n")
    bad = [r["origin"] for r in results if not all(r["checks"].values())]
    if bad or not origins:
        print("package acceptance failed: " + ", ".join(bad or ["no origins"]), file=sys.stderr)
    return 1 if bad or not origins else 0


if __name__ == "__main__":
    sys.exit(main())
