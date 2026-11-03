#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
"""DRAFT generator for the daemonarchy-ports retained-project manifest and profile.

Run with the Literate AI runtime's interpreter (it imports literate_ai) from the
project root, after closure-inventory.py and stage-closure.py have produced the untracked ./closure:

    litai-venv/bin/python Tools/daemonarchy/retained/draft-retained-inputs.py \
        --manifest-out /abs/path/outside/tree/manifest.draft.json

It writes the two tracked draft authority files next to itself (profile and test
policy) and the full inventory manifest (about 180,000 members, NOT tracked) to
--manifest-out.  It reads, hashes and lists; it executes nothing from the tree and
asserts no build, test or acceptance result.  See PROVENANCE.md for every unknown.
"""
import argparse
import ast
import hashlib
import importlib.util
import json
import os
import stat
import sys
from pathlib import Path

from literate_ai.contracts.execution_dispatch import ExecutionWorker, ExecutionWorkerKind
from literate_ai.contracts.identity import ContentIdentity, canonical_identity
from literate_ai.contracts.retained_project import (
    RetainedProjectLimits,
    RetainedProjectManifest,
    RetainedProjectMember,
    RetainedProjectTreePolicy,
)
from literate_ai.contracts.retained_project_lifecycle import RetainedProjectProfile
from literate_ai.contracts.retained_project_tests import RetainedProjectTestPolicy
from literate_ai.projects import ProjectConfigurationStore

HERE = "Tools/daemonarchy/retained"
PROFILE = HERE + "/profile.draft.json"
POLICY = HERE + "/test-policy.draft.json"
TARGET = "freebsd"
EXCLUDED_TOP = {".git", ".literate", "literate.project.json", "retained-out", "_build",
                "verification"}  # verification/: the withheld acceptance policy, never a manifest root  # _build: litai cache, git-ignored
# Only the portable suite the repository's CI runs verbatim
# (.github/workflows/producer-inputs-native.yml); the native/hosted-VM step and
# test_omarchy_debug_status.py are not part of this draft's inventory.
TEST_FILES = ("Tools/daemonarchy/test_observe_unit_inputs.py",)
REPORT_TOOL = HERE + "/unittest-report.py"
RELEASE = "literate.release.json"
ORIGINS = HERE + "/origins.draft.list"
CLOSURE = HERE + "/closure-inventory.json"
TC = "closure/toolchain/bin/"
JAIL, PTREE = "16r2303b36", "retained"   # bulk.env; poudriere names the package dir JAIL-PTREE
DOC_INCLUDE = ("docs/**/*.md", "*.md", "Tools/**/*.md")
BUILD_AUX = (HERE + "/run-bulk.sh", HERE + "/bulk.env", HERE + "/ambient-preflight.py",
             HERE + "/accept-packages.py", HERE + "/closure-inventory.json",
             HERE + "/closure-spec.json", HERE + "/docs-tree-check.py",
             HERE + "/daemonarchy-verify.sh")


def role_of(path: str) -> str:
    if path.startswith("closure/toolchain/"):
        return "toolchain"
    if path.startswith("closure/dependency/"):
        return "dependency"
    if path == RELEASE:
        return "release-policy"
    if path == POLICY:
        return "test-policy"
    if path in (PROFILE, ORIGINS, "Makefile") or path in BUILD_AUX or path.startswith("Mk/"):
        return "build-definition"
    if path in TEST_FILES or path == REPORT_TOOL:
        return "test-definition"
    if path.startswith("docs/") or path.endswith(".md") and "/" not in path:
        return "docs-definition"
    return "source"


def sha256(path: Path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def inventory(root: Path, roots):
    members, dirs = [], []

    def visit(rel: str):
        full = root / rel
        st = os.lstat(full)
        if stat.S_ISLNK(st.st_mode):
            target = os.readlink(full)
            data = target.encode()
            members.append(RetainedProjectMember(rel, len(data), 0o120000,
                hashlib.sha256(data).hexdigest(), role_of(rel), symlink_target=target))
        elif stat.S_ISDIR(st.st_mode):
            dirs.append(rel)
            for name in os.listdir(full):
                visit(rel + "/" + name)
        elif stat.S_ISREG(st.st_mode):
            mode = 0o755 if st.st_mode & 0o111 else 0o644
            members.append(RetainedProjectMember(rel, st.st_size, mode,
                sha256(full), role_of(rel)))
        else:
            raise SystemExit("special file in retained roots: " + rel)

    for r in roots:
        visit(r)
    members.sort(key=lambda m: m.path)
    return tuple(members), tuple(sorted(dirs))


def test_cases():
    cases = []
    for name in TEST_FILES:
        tree = ast.parse(Path(name).read_text())
        module = Path(name).stem
        for node in tree.body:
            if isinstance(node, ast.ClassDef):
                for item in node.body:
                    if isinstance(item, ast.FunctionDef) and item.name.startswith("test"):
                        cases.append(f"{module}.{node.name}.{item.name}")
    return tuple(sorted(set(cases)))


def doc_files(root: Path):
    spec = importlib.util.spec_from_file_location("docs_tree_check", root / HERE / "docs-tree-check.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    found = []
    for base, dirs, names in os.walk(root):
        dirs[:] = sorted(d for d in dirs if d not in (".git", "_build", "closure", "retained-out"))
        for n in sorted(names):
            rel = os.path.relpath(os.path.join(base, n), root)
            if module.matches(rel, DOC_INCLUDE):
                found.append(rel)
    return found


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--manifest-out", required=True)
    args = ap.parse_args()
    root = Path.cwd().resolve()
    roots = tuple(sorted(n for n in os.listdir(root) if n not in EXCLUDED_TOP))
    definition = ProjectConfigurationStore(root).read().definition
    closure = json.loads((root / CLOSURE).read_text())
    if closure["problems"]:
        raise SystemExit("closure inventory has unresolved problems")

    # 1. test policy: statically collected case names only; outcomes unknown.
    cases = test_cases()
    policy = RetainedProjectTestPolicy.from_dict({
        "schema": "literate-ai/retained-project-test-policy@1",
        "project_policy_identity": definition.test_receipt_policy.identity.uri,
        "cases": [{"id": c, "action": "draft-original-unit-tests", "skip_reason": None}
                  for c in cases],
        "reports": [{"action": "draft-original-unit-tests",
                     "path": "tests/unit-report.json"}],
    })
    (root / POLICY).write_text(json.dumps(policy.to_dict(), indent=2) + "\n")

    # 2. provisional manifest (no profile yet) -> role-closure identities.
    limits = RetainedProjectLimits(1_000_000, 512 * 1024 * 1024, 4 * 1024**3)
    tree = RetainedProjectTreePolicy(case_sensitive=True)
    profile_stub = root / PROFILE
    profile_stub.write_text("{}\n")
    members, dirs = inventory(root, roots)
    provisional = RetainedProjectManifest(roots, members, limits, dirs, (), tree)
    by_path = {m.path: m for m in members}

    def tool(name):
        m = by_path[TC + name]
        return {"name": name, "path": m.path, "identity": "sha256:" + m.sha256}

    py = TC + "python3.12"
    docs_files = doc_files(root)
    ambient_items = ["ambient:" + a["name"] for a in closure["ambient"]]
    origins = [l.strip() for l in (root / ORIGINS).read_text().splitlines() if l.strip()]

    worker = ExecutionWorker("local", ExecutionWorkerKind.LOCAL, target_profile=TARGET)
    release_member = next(m for m in members if m.path == RELEASE)
    acceptance_path = root / "verification" / "retained-project-acceptance.json"
    acceptance_identity = ("sha256:" + hashlib.sha256(acceptance_path.read_bytes()).hexdigest()
                           if acceptance_path.is_file() else None)
    policy_member = next(m for m in members if m.path == POLICY)
    profile = {
        "schema": "literate-ai/retained-project-profile@1",
        "authority_mode": "retained-project",
        "target": TARGET,
        "worker_identity": worker.identity.uri,
        "toolchain_identity": provisional.role_identity({"sdk", "toolchain"}).uri,
        "dependency_identity": provisional.role_identity({"dependency"}).uri,
        "test_policy_identity": "sha256:" + policy_member.sha256,
        "release_policy_identity": "sha256:" + release_member.sha256,
        **({"acceptance_policy_identity": acceptance_identity} if acceptance_identity else {}),
        "tools": [tool(t["name"]) for t in closure["tools"] if t["role"] == "toolchain"],
        "runtime_library_paths": ["closure/toolchain/lib", "closure/dependency/lib"],
        "actions": [
            # Re-observes the ambient host inputs (registry, build-jail anchors, git helper
            # directories, dynamic linker) against closure-inventory.json.  Read-only.
            {"id": "draft-ambient-preflight", "stage": "stage", "tool": "python3.12",
             "argv": [py, "-B", HERE + "/ambient-preflight.py", "--inventory", CLOSURE,
                      "--output", "{output_root}/preflight/ambient.json"],
             "requires": [], "inventory": ambient_items, "outputs": ["preflight"],
             "timeout_seconds": 3600},
            # `poudriere bulk -j JAIL -p PTREE -b latest -t -f <list>` (daemonarchy
            # HACKING.md "Build the packages" step 4) through run-bulk.sh, which maps the
            # registry, data directory and ports tree onto the run (see PROVENANCE.md).
            # Root is required.  Origin: the first closure in docs/user/ports-units.md.
            {"id": "draft-poudriere-build", "stage": "build", "tool": "sh",
             "argv": [TC + "sh", HERE + "/run-bulk.sh", "{output_root}/build"],
             "requires": ["draft-ambient-preflight"], "inventory": list(origins),
             "outputs": ["build"], "timeout_seconds": 86400},
            # CI (producer-inputs-native.yml) runs this file verbatim; unittest-report.py
            # only adds the report the policy names.  test needs `git` (captured).
            {"id": "draft-original-unit-tests", "stage": "test", "tool": "python3.12",
             "argv": [py, "-B", REPORT_TOOL, "--test-file", TEST_FILES[0],
                      "--output", "{output_root}/tests/unit-report.json"],
             "requires": ["draft-poudriere-build"], "inventory": list(cases),
             "outputs": ["tests"], "timeout_seconds": 3600},
            # Whole-tree check of the project-authored Markdown (docs/, root, Tools/).
            # Port Makefiles, pkg-descr files and the Mk/ framework have no docs check.
            {"id": "draft-docs-tree", "stage": "docs", "tool": "python3.12",
             "argv": [py, "-B", HERE + "/docs-tree-check.py", "--root", "."]
                     + [a for p in DOC_INCLUDE for a in ("--include", p)]
                     + ["--output", "{output_root}/docs-tree/report.json"],
             "requires": ["draft-poudriere-build"],
             "inventory": ["docs-tree:" + f for f in docs_files],
             "outputs": ["docs-tree"], "timeout_seconds": 3600},
            # STATIC package acceptance by a verifier separate from poudriere.  Installation
            # and the ports' own test results are not examined (UNKNOWN).
            {"id": "draft-accept-packages", "stage": "accept", "tool": "python3.12",
             "argv": [py, "-B", HERE + "/accept-packages.py", "--packages",
                      "{output_root}/build/poudriere-data/packages/%s-%s" % (JAIL, PTREE),
                      "--origins", ORIGINS, "--output", "{output_root}/acceptance/packages.json"],
             "requires": ["draft-ambient-preflight", "draft-poudriere-build",
                          "draft-original-unit-tests", "draft-docs-tree"],
             "inventory": ["package:" + o for o in origins],
             "outputs": ["acceptance"], "timeout_seconds": 3600},
        ],
        "package_inventory": ["build"],
    }
    RetainedProjectProfile.from_dict(profile)  # schema validation only
    profile_stub.write_text(json.dumps(profile, indent=2) + "\n")

    # 3. final manifest including the profile bytes.
    members, dirs = inventory(root, roots)
    manifest = RetainedProjectManifest(roots, members, limits, dirs, (), tree)
    out = Path(args.manifest_out)
    out.write_text(json.dumps(manifest.to_dict(), separators=(",", ":")) + "\n")
    print(json.dumps({
        "roots": len(roots), "members": len(members), "directories": len(dirs),
        "symlinks": sum(1 for m in members if m.symlink_target is not None),
        "total_bytes": sum(m.size for m in members),
        "test_cases": len(cases), "tools": len(profile["tools"]), "doc_files": len(docs_files),
        "manifest_identity": manifest.identity.uri,
        "profile_identity": RetainedProjectProfile.from_dict(profile).identity.uri,
        "worker_identity": worker.identity.uri,
        "toolchain_identity": profile["toolchain_identity"],
        "dependency_identity": profile["dependency_identity"],
        "test_policy_identity": profile["test_policy_identity"],
        "release_policy_identity": profile["release_policy_identity"],
    }, indent=2))


if __name__ == "__main__":
    sys.exit(main())
