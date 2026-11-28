#!/usr/bin/env python3
"""Exercise the actual driver's NVRAM parser with bounded child processes.

Pass a patched src/pcie.c. Only kernel allocation is replaced by calloc; the
parser body is extracted verbatim. This is a parser regression, not radio or
complete kernel-module qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shlex
import struct
import subprocess
import tempfile


def expected(records):
    data = b"".join(record + b"\0" for record in records)
    data += b"\0" * (-len(data) % 4)
    words = len(data) // 4
    return data + struct.pack("<I", ((~words & 0xFFFF) << 16) | words)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("--cc", default="cc")
    parser.add_argument("--cflags", default="-O2 -Wall -Wextra -Werror")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    source = args.source.read_text()
    start = source.index("void *\nbrcmf_nvram_parse(")
    end = source.index("\n}\n", start) + 3
    function = source[start:end]
    cases = [
        ("empty", b"", None),
        ("record-eof", b"a=1", [b"a=1"]),
        ("record-lf", b"a=1\n", [b"a=1"]),
        ("record-crlf", b"a=1\r\n", [b"a=1"]),
        ("two-records", b"a=1\nb=2", [b"a=1", b"b=2"]),
        ("comment-lf", b"# comment\n", []),
        ("comment-eof", b"# comment", []),
        ("comment-marker-eof", b"#", []),
        ("trailing-comment-eof", b"a=1\n# comment", [b"a=1"]),
        ("trailing-comment-crlf-eof", b"a=1\r\n# comment", [b"a=1"]),
        ("blank-lines", b"\r\n\na=1\n\n", [b"a=1"]),
        ("comments-between-records", b"# first\r\na=1\n# middle\nb=2\n", [b"a=1", b"b=2"]),
        ("equals-in-value", b"a=x=y\n", [b"a=x=y"]),
    ]
    shim = '''#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define M_BRCMFMAC 0
#define M_NOWAIT 0
#define M_ZERO 0
#define malloc(size, type, flags) calloc(1, (size))
'''
    shim += function
    shim += '''
#undef malloc
int main(int argc, char **argv) {
    unsigned char data[65536];
    uint32_t len = 0;
    FILE *f;
    size_t size;
    void *result;
    if (argc != 2 || (f = fopen(argv[1], "rb")) == NULL) return 2;
    size = fread(data, 1, sizeof(data), f);
    if (ferror(f) || !feof(f)) return 3;
    fclose(f);
    result = brcmf_nvram_parse(data, size, &len);
    if (len && fwrite(result, 1, len, stdout) != len) return 4;
    free(result);
    return 0;
}
'''
    review = {
        "source_sha256": hashlib.sha256(args.source.read_bytes()).hexdigest(),
        "function_sha256": hashlib.sha256(function.encode()).hexdigest(),
        "scope": "Verbatim production parser with allocation shim; no SDIO/radio execution",
        "cases": [],
    }
    with tempfile.TemporaryDirectory(prefix="brcmfmac-nvram-") as temporary:
        root = Path(temporary)
        (root / "probe.c").write_text(shim)
        command = shlex.split(args.cc) + shlex.split(args.cflags) + [str(root / "probe.c"), "-o", str(root / "probe")]
        compiled = subprocess.run(command, capture_output=True, text=True, timeout=60)
        review["compile"] = {"argv": command, "exit": compiled.returncode, "stdout": compiled.stdout, "stderr": compiled.stderr}
        if compiled.returncode == 0:
            for name, data, records in cases:
                path = root / "input"
                path.write_bytes(data)
                row = {"name": name, "input_hex": data.hex()}
                wanted = b"" if records is None else expected(records)
                row["expected_hex"] = wanted.hex()
                try:
                    ran = subprocess.run([str(root / "probe"), str(path)], capture_output=True, timeout=2)
                    row.update(exit=ran.returncode, stdout_hex=ran.stdout.hex(), stderr=ran.stderr.decode(errors="replace"))
                    row["pass"] = ran.returncode == 0 and ran.stdout == wanted
                except subprocess.TimeoutExpired:
                    row.update(timeout_seconds=2, **{"pass": False})
                review["cases"].append(row)
    review["pass"] = compiled.returncode == 0 and len(review["cases"]) == len(cases) and all(row["pass"] for row in review["cases"])
    args.output.write_text(json.dumps(review, indent=2) + "\n")
    print(json.dumps({"pass": review["pass"], "cases": len(review["cases"]), "failed": [row["name"] for row in review["cases"] if not row["pass"]]}))
    return 0 if review["pass"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
