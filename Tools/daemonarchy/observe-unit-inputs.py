#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
"""Observe one exact native port's evaluated build/runtime inputs without building.

Requires an allocated unprivileged FreeBSD worker, a full exact source checkout,
an explicit architecture/OSVERSION/flavor/complete option context, and a fresh
external output directory. Makefile evaluation can execute programs; it requires
--acknowledge-make-evaluation. No dependencies are installed, no port targets are
built and no existing work, options, packages or distfiles are removed.

The observation is not a complete recursive graph, a canonical Component
registration, retained-project admission, ABI proof or cache-hit authorization.
Canonical per-unit retained manifests/profiles remain execution authority.
The context must declare package_database={mode:empty-diagnostic,identity:null}
or mode:snapshot with a pinned sha256 identity and --package-snapshot binding.
Empty diagnostic DB state never reproduces selected installed-package facts.
Snapshots must be sealed/read-only and offline (no WAL/SHM/journal); only private
copies can be modified. Complete selected option/flavor context remains explicit.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import resource
import signal
import shutil
import subprocess
import sys
import time

MAKE = "/usr/bin/make"
VARIABLES = ("PKGNAME", "PKGORIGIN", "ARCH", "OPSYS", "OSVERSION", "FLAVOR", "FLAVORS",
             "COMPLETE_OPTIONS_LIST", "PORT_OPTIONS", "BUILD_DEPENDS", "LIB_DEPENDS", "RUN_DEPENDS",
             "FETCH_DEPENDS", "EXTRACT_DEPENDS", "PATCH_DEPENDS", "TEST_DEPENDS", "USES",
             "CC", "CXX", "CPP", "LD", "CFLAGS", "CXXFLAGS", "CPPFLAGS", "LDFLAGS",
             "CONFIGURE_ARGS", "CONFIGURE_ENV", "MAKE_ARGS", "MAKE_ENV", "DISTFILES",
             "DISTINFO_FILE", "MASTER_SITES", "GH_ACCOUNT", "GH_PROJECT", "GH_TAGNAME", "GH_TUPLE",
             "LOCALBASE", "PREFIX", "MASTERDIR", "FILESDIR", "PATCHDIR", "SCRIPTDIR",
             "PLIST", "PKGINSTALL", "PKGDEINSTALL", "PKGMESSAGE", ".MAKE.MAKEFILES")
DEPENDENCIES = {"BUILD_DEPENDS": ("build",), "LIB_DEPENDS": ("build", "runtime"),
                "RUN_DEPENDS": ("runtime",), "FETCH_DEPENDS": ("build",),
                "EXTRACT_DEPENDS": ("build",), "PATCH_DEPENDS": ("build",), "TEST_DEPENDS": ("test",)}
ORIGIN = re.compile(r"[a-z0-9][a-z0-9+_.-]*/[A-Za-z0-9][A-Za-z0-9+_.-]*(?:@[A-Za-z0-9+_.-]+)?\Z")
OPTION = re.compile(r"[A-Z0-9][A-Z0-9_]*\Z")
ALLOWED_VARIABLES = {"CC", "CXX", "CPP", "LD", "CFLAGS", "CXXFLAGS", "CPPFLAGS", "LDFLAGS",
                     "DEFAULT_VERSIONS", "LOCALBASE", "PREFIX", "BINARY_ALIAS"}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def pairs(items):
    result = {}
    for key, value in items:
        require(key not in result, "duplicate JSON field: " + key)
        result[key] = value
    return result


def read_context(path):
    with Path(path).open("rb") as stream:
        data = stream.read(1024 * 1024 + 1)
    require(len(data) <= 1024 * 1024, "context exceeds byte bound")
    return json.loads(data, object_pairs_hook=pairs)


def validate_context(context):
    require(isinstance(context, dict) and set(context) == {"architecture", "osversion", "flavor", "options", "make_variables", "package_database"},
            "explicit complete native context required")
    require(context["architecture"] in {"amd64", "aarch64", "i386", "armv7", "riscv64"}, "unknown native architecture")
    require(type(context["osversion"]) is int and context["osversion"] > 0, "explicit OSVERSION required")
    require(context["flavor"] is None or isinstance(context["flavor"], str) and
            re.fullmatch(r"[A-Za-z0-9+_.-]+", context["flavor"]), "invalid native port flavor")
    require(isinstance(context["options"], dict) and all(isinstance(k, str) and OPTION.fullmatch(k) and
            type(v) is bool for k, v in context["options"].items()), "complete Boolean port options required")
    require(isinstance(context["make_variables"], dict) and set(context["make_variables"]) <= ALLOWED_VARIABLES,
            "unsupported native make variable")
    database=context["package_database"]
    require(isinstance(database,dict) and set(database)=={"mode","identity"} and
            database["mode"] in {"empty-diagnostic","snapshot"}, "explicit package database context required")
    require(database["identity"] is None if database["mode"]=="empty-diagnostic" else
            isinstance(database["identity"],str) and re.fullmatch(r"sha256:[0-9a-f]{64}",database["identity"]),
            "package database snapshot identity required")
    for value in context["make_variables"].values():
        require(isinstance(value, str) and len(value) <= 8192 and not any(ord(c) < 32 for c in value)
                and not any(c in value for c in "$`"), "make context contains unreviewed expansion")


def git(repo, *args):
    return subprocess.run(["git", "-C", str(repo), *args], check=True, timeout=20,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                          env={**os.environ, "GIT_NO_LAZY_FETCH": "1", "GIT_TERMINAL_PROMPT": "0", "GIT_OPTIONAL_LOCKS":"0"}).stdout


def dependency_edges(values):
    edges = []
    for variable, kinds in DEPENDENCIES.items():
        for item in values[variable].split():
            fields = item.split(":")
            require(len(fields) in (2, 3) and fields[0] and ORIGIN.fullmatch(fields[1]),
                    "unresolved native dependency expression in " + variable + ": " + item)
            for kind in kinds:
                edges.append({"kind": kind, "declaration": variable, "requirement": fields[0],
                              "origin": fields[1], "target": fields[2] if len(fields) == 3 else None})
    return sorted(edges, key=lambda e: (e["kind"], e["declaration"], e["origin"], e["requirement"], e["target"] or ""))


def package_database_identity(directory):
    """Hash a sealed offline snapshot; never copy a live SQLite journal state."""
    require(directory.is_dir() and not directory.is_symlink(), "direct installed package snapshot required")
    files=[]
    total=0
    for path in sorted(directory.iterdir()):
        require(path.is_file() and not path.is_symlink() and
                re.fullmatch(r"[A-Za-z0-9_.+-]+",path.name), "unsupported package snapshot entry")
        require(not path.name.endswith(("-wal","-shm","-journal")), "live SQLite journals require an atomic offline snapshot")
        before=path.stat()
        readonly_mount=bool(os.statvfs(path).f_flag & getattr(os,"ST_RDONLY",1))
        require(not before.st_mode & 0o222 or readonly_mount, "package snapshot files must be sealed read-only")
        total+=before.st_size
        require(len(files)<128 and total<=512*1024*1024, "package database snapshot exceeds byte/file budget")
        identity=hashlib.sha256()
        with path.open("rb") as stream:
            for block in iter(lambda:stream.read(1024*1024),b""):
                identity.update(block)
        after=path.stat()
        require((before.st_dev,before.st_ino,before.st_size,before.st_mtime_ns,before.st_ctime_ns)==
                (after.st_dev,after.st_ino,after.st_size,after.st_mtime_ns,after.st_ctime_ns),
                "package snapshot changed during observation")
        files.append({"name":path.name,"size":before.st_size,"sha256":identity.hexdigest()})
    require(any(f["name"]=="local.sqlite" for f in files), "installed local.sqlite snapshot required")
    data=json.dumps({"schema":"daemonarchy/installed-package-database-snapshot@1","files":files},
                    sort_keys=True,separators=(",",":")).encode()
    return files,"sha256:"+sha(data)


def stage_package_database(output, declaration, snapshot):
    if declaration["mode"]=="empty-diagnostic":
        require(snapshot is None,"empty diagnostic mode must not imply an installed snapshot")
        return {"mode":"empty-diagnostic","identity":None,"selected_release_context_verified":False,
                "semantics":"fresh empty pkgdb; not selected installed-package context"}
    require(snapshot is not None,"bind an explicit installed-package snapshot")
    files,identity=package_database_identity(snapshot)
    require(identity==declaration["identity"],"installed-package snapshot identity changed")
    for entry in files:
        destination=output/"pkgdb"/entry["name"]
        with (snapshot/entry["name"]).open("rb") as source,destination.open("xb") as target:
            shutil.copyfileobj(source,target,1024*1024)
        with destination.open("rb") as stream:
            actual=hashlib.file_digest(stream,"sha256").hexdigest()
        require(actual==entry["sha256"],"installed package snapshot changed during copy")
        destination.chmod(0o600)  # Only the private copy can receive pkg scratch writes.
    require(package_database_identity(snapshot)[1]==identity,"package snapshot changed during staging")
    return {"mode":"snapshot-copy","identity":identity,"files":files,
            "selected_release_context_verified":False,
            "semantics":"exact offline snapshot; release/toolchain selection still requires canonical provenance"}


def command(repo, origin, output, context):
    args = [MAKE, "-m", str(repo/"Mk"), "-m", "/usr/share/mk", "-C", str(repo / origin), "PORTSDIR="+str(repo), "__MAKE_CONF=/dev/null", "SRCCONF=/dev/null",
            "SRC_ENV_CONF=/dev/null", "BATCH=yes", "NO_DIALOG=yes", "OPSYS=FreeBSD",
            "ARCH=" + context["architecture"], "OSVERSION=" + str(context["osversion"]),
            "OPTIONS_OVERRIDE=" + " ".join(sorted(k for k, v in context["options"].items() if v))]
    for variable, directory in (("WRKDIRPREFIX", "work"), ("DISTDIR", "distfiles"), ("PACKAGES", "packages"),
                                ("PORT_DBDIR", "options"), ("PKG_DBDIR", "pkgdb"), ("MAKEOBJDIRPREFIX", "obj")):
        args.append(variable + "=" + str(output / directory))
    if context["flavor"] is not None:
        args.append("FLAVOR=" + context["flavor"])
    args.extend(k + "=" + v for k, v in sorted(context["make_variables"].items()))
    for variable in VARIABLES:
        args.extend(("-V", variable))
    return args


def output_limit():
    resource.setrlimit(resource.RLIMIT_FSIZE, (16 * 1024 * 1024, 16 * 1024 * 1024))


def query_environment(output):
    env = {"PATH": "/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin",
           "HOME": str(output / "home"), "TMPDIR": str(output / "tmp"), "LC_ALL": "C", "TZ": "UTC",
           "PKG_DBDIR":str(output/"pkgdb"), "PKG_CACHEDIR":str(output/"pkgcache"),
           "PORT_DBDIR":str(output/"options"), "DISTDIR":str(output/"distfiles"),
           "PACKAGES":str(output/"packages"), "WRKDIRPREFIX":str(output/"work")}
    return env


def query(args, output, timeout):
    env=query_environment(output)
    with (output / "make.stdout").open("xb") as stdout, (output / "make.stderr").open("xb") as stderr:
        process = subprocess.Popen(args, env=env, stdout=stdout, stderr=stderr, start_new_session=True, preexec_fn=output_limit)
        try:
            code = process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            raise TimeoutError("native make metadata deadline expired; partial evidence retained")
        finally:
            if process.poll() is None:
                try:
                    os.killpg(process.pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
                process.wait()
            (output / "make.exit").write_text(str(process.returncode) + "\n")
    require(code == 0, "native make metadata exited nonzero; logs retained")
    with (output / "make.stdout").open("rb") as stream:
        data = stream.read(16 * 1024 * 1024 + 1)
    require(len(data) <= 16 * 1024 * 1024, "native metadata exceeds byte budget")
    return decode_values(data)


def decode_values(data):
    require(len(data)<=16*1024*1024,"native metadata exceeds byte bound")
    lines=data.decode().splitlines()
    require(len(lines)==len(VARIABLES),"native metadata variable inventory is incomplete or multiline")
    return dict(zip(VARIABLES,lines))


def input_record(repo, revision, path):
    require(path.is_file() or path.is_symlink(), "native input is absent: " + str(path))
    resolved = path.resolve(strict=True)
    if path.is_symlink():
        data = os.readlink(path).encode()
    else:
        with path.open("rb") as stream:
            data = stream.read(32 * 1024 * 1024 + 1)
        require(len(data) <= 32 * 1024 * 1024, "metadata input exceeds byte bound")
    if path.is_relative_to(repo):
        relative = path.relative_to(repo).as_posix()
        entries = git(repo, "ls-tree", "-z", revision, "--", relative).split(b"\0")
        require(len(entries) == 2 and entries[0], "native make input is not selected Git content: " + relative)
        metadata, name = entries[0].split(b"\t", 1)
        mode, kind, oid = metadata.decode().split()
        require(kind == "blob" and mode in {"100644", "100755", "120000"}, "unsupported native Git input")
        require(git(repo, "cat-file", "blob", oid) == data, "native make input differs from selected Git object: " + relative)
        record = {"path": relative, "sha256": sha(data), "git_blob": oid, "git_mode": mode, "owner": "ports-source-unregistered"}
        if path.is_symlink():
            record["symlink_target"] = input_record(repo, revision, resolved)
        return record
    record = {"external_path": str(path), "sha256": sha(data), "owner": "native-SDK-unadmitted"}
    if path.is_symlink():
        record["symlink_target"] = input_record(repo, revision, resolved)
    return record


def observe(repo, revision, origin, context, output, *, acknowledged, timeout=20, package_snapshot=None):
    require(acknowledged, "Makefile evaluation requires --acknowledge-make-evaluation")
    require(platform.system() == "FreeBSD" and os.geteuid() != 0,
            "use an allocated unprivileged native FreeBSD worker")
    validate_context(context)
    require(ORIGIN.fullmatch(origin) and "@" not in origin, "select category/origin and flavor separately")
    require(isinstance(revision, str) and re.fullmatch(r"[0-9a-f]{40}", revision), "exact source commit required")
    require(type(timeout) is int and 1 <= timeout <= 120, "bounded metadata deadline required")
    repo, output = repo.resolve(strict=True), output.absolute()
    require(not output.is_relative_to(repo) and not repo.is_relative_to(output), "output must be outside source and its ancestors")
    require(output.parent.resolve(strict=True) == output.parent, "output parent must be direct")
    require((repo / origin).is_dir() and (repo / origin).resolve() == repo / origin,
            "native origin must be a direct source directory")
    require(git(repo, "rev-parse", "HEAD").decode().strip() == revision, "source checkout differs from selected revision")
    git(repo, "diff", "--quiet", "--no-ext-diff", "--no-textconv", revision, "--", origin, "Mk", "Templates", "Keywords")
    output.mkdir(mode=0o700)  # Exclusive: retain all prior evidence and outputs.
    for name in ("home", "tmp", "work", "distfiles", "packages", "options", "pkgdb", "pkgcache", "obj"):
        (output / name).mkdir()
    args = command(repo, origin, output, context)
    (output / "request.json").write_text(json.dumps({"revision": revision, "origin": origin,
                                                    "context": context, "argv": args}, sort_keys=True) + "\n")
    started = time.monotonic()
    try:
        database_context=stage_package_database(output,context["package_database"],package_snapshot)
        values = query(args, output, timeout)
        require(values["ARCH"] == context["architecture"] and values["OPSYS"] == "FreeBSD" and
                values["OSVERSION"] == str(context["osversion"]), "evaluated target/ABI differs from selected context")
        require(values["FLAVOR"] == (context["flavor"] or ""), "explicit native flavor does not match evaluated flavor")
        require(set(values["COMPLETE_OPTIONS_LIST"].split()) == set(context["options"]),
                "native options inventory is incomplete; supply every evaluated option explicitly")
        require(set(values["PORT_OPTIONS"].split()) == {k for k, v in context["options"].items() if v},
                "evaluated options differ from selected context")
        edges = dependency_edges(values)
        roots = {origin}
        unresolved = []
        for variable in ("MASTERDIR", "FILESDIR", "PATCHDIR", "SCRIPTDIR"):
            if values[variable]:
                directory = Path(values[variable])
                if not directory.is_absolute():
                    directory = repo / origin / directory
                directory = directory.resolve()
                if directory.is_relative_to(repo):
                    roots.add(directory.relative_to(repo).as_posix())
                else:
                    unresolved.append({"variable": variable, "path": str(directory), "reason": "external input ownership unadmitted"})
        names = sorted({raw for root in roots for raw in git(repo, "ls-tree", "-r", "-z", "--name-only", revision, "--", root).decode().split("\0") if raw})
        require(0 < len(names) <= 10000, "port subtree exceeds bounded input inventory")
        inputs = []
        for path in names:
            require(time.monotonic() - started <= timeout + 60, "metadata input observation exceeded total deadline")
            inputs.append(input_record(repo, revision, repo / path))
        seen = {e.get("path") for e in inputs}
        referenced = values[".MAKE.MAKEFILES"].split()
        for variable in ("DISTINFO_FILE", "PLIST", "PKGINSTALL", "PKGDEINSTALL", "PKGMESSAGE"):
            if values[variable]:
                path = Path(values[variable])
                if not path.is_absolute():
                    path = repo / origin / path
                if path.is_file() or path.is_symlink():
                    referenced.append(str(path))
                else:
                    unresolved.append({"variable": variable, "path": str(path), "reason": "generated or absent input requires producer"})
        for path in referenced:
            require(time.monotonic() - started <= timeout + 60, "metadata input observation exceeded total deadline")
            source = Path(path)
            if not source.is_absolute():
                source = repo / origin / source
            source = Path(os.path.abspath(source))
            record = input_record(repo, revision, source)
            if record.get("path", record.get("external_path")) not in seen:
                inputs.append(record)
                seen.add(record.get("path", record.get("external_path")))
        require(values[".MAKE.MAKEFILES"].split(), "native make omitted included build inputs")
        git(repo, "diff", "--quiet", "--no-ext-diff", "--no-textconv", revision, "--", *sorted(roots), "Mk", "Templates", "Keywords")
        result = {"schema": "daemonarchy/ports-unit-input-observation@1", "revision": revision,
                  "git_tree": git(repo, "rev-parse", revision + "^{tree}").decode().strip(),
                  "origin": origin, "context": context, "package_database_context":database_context,
                  "options_context":"explicit complete OPTIONS_OVERRIDE; existing options database is not consulted",
                  "installed_toolchain_filesystem_context_verified":False,
                  "repository_configuration_context_verified":False,
                  "variables": values, "dependencies": edges,
                  "inputs": inputs, "unresolved_inputs": unresolved, "elapsed_milliseconds": int((time.monotonic()-started)*1000),
                  "recursive_closure_complete": False, "canonical_ownership_registered": False,
                  "native_profile_admitted": False, "cache_admission": False,
                  "remaining_gates": ["recursive selected dependency closure", "compiler/sysroot/generated input observation", "fetched distfile bytes and signatures",
                                      "canonical Component ownership and retained profiles", "native products/ABI/receipts"]}
        (output / "observation.json").write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
        return result
    except BaseException as error:
        (output / "failure.json").write_text(json.dumps({"error": str(error), "cache_admission": False,
                                                        "recursive_closure_complete": False}) + "\n")
        raise


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, required=True)
    parser.add_argument("--revision", required=True)
    parser.add_argument("--origin", required=True)
    parser.add_argument("--context", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--timeout", type=int, default=20)
    parser.add_argument("--package-snapshot",type=Path,help="sealed offline installed-pkgdb snapshot; copied privately")
    parser.add_argument("--acknowledge-make-evaluation", action="store_true")
    args = parser.parse_args()
    try:
        report = observe(args.repo, args.revision, args.origin, read_context(args.context), args.output,
                         acknowledged=args.acknowledge_make_evaluation, timeout=args.timeout, package_snapshot=args.package_snapshot)
        print(json.dumps({"origin": report["origin"], "dependencies": len(report["dependencies"]),
                          "recursive_closure_complete": False, "cache_admission": False}))
        return 0
    except (ValueError, OSError, TimeoutError, subprocess.SubprocessError) as error:
        print("ports input observation refused: " + str(error), file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
