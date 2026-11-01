#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
"""DRAFT generator for the daemonarchy-ports retained-project manifest and profile.

Run with the Literate AI runtime's interpreter (it imports literate_ai) from the
project root, after staging the draft toolchain under ./sdk/bin (untracked):

    litai-venv/bin/python Tools/daemonarchy/retained/draft-retained-inputs.py \
        --manifest-out /abs/path/outside/tree/manifest.draft.json

It writes the two tracked draft authority files next to itself (profile and test
policy) and the full inventory manifest (about 180,000 members, NOT tracked) to
--manifest-out.  It reads, hashes and lists; it executes nothing from the tree and
asserts no build, test or acceptance result.  See DRAFT.md for every unknown.
"""
import argparse
import ast
import hashlib
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
EXCLUDED_TOP = {".git", ".literate", "literate.project.json", "retained-out"}
TEST_FILES = (
    "Tools/daemonarchy/test_observe_unit_inputs.py",
    "Tools/daemonarchy/test_observe_unit_inputs_native.py",
    "Tools/daemonarchy/test_omarchy_debug_status.py",
)


def role_of(path: str) -> str:
    if path.startswith("sdk/"):
        return "toolchain"
    if path == POLICY:
        return "test-policy"
    if path == PROFILE or path == "Makefile" or path.startswith("Mk/"):
        return "build-definition"
    if path in TEST_FILES or path == "Tools/daemonarchy/check-arch-deps.sh":
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


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--manifest-out", required=True)
    args = ap.parse_args()
    root = Path.cwd().resolve()
    roots = tuple(sorted(n for n in os.listdir(root) if n not in EXCLUDED_TOP))
    definition = ProjectConfigurationStore(root).read().definition

    # 1. test policy: statically collected case names only; outcomes unknown.
    cases = test_cases()
    policy = RetainedProjectTestPolicy.from_dict({
        "schema": "literate-ai/retained-project-test-policy@1",
        "project_policy_identity": definition.test_receipt_policy.identity.uri,
        "cases": [{"id": c, "action": "draft-original-unit-tests", "skip_reason": None}
                  for c in cases],
        "reports": [{"action": "draft-original-unit-tests",
                     "path": "retained-out/tests/unit-report.json"}],
    })
    (root / POLICY).write_text(json.dumps(policy.to_dict(), indent=2) + "\n")

    # 2. provisional manifest (no profile yet) -> role-closure identities.
    limits = RetainedProjectLimits(1_000_000, 512 * 1024 * 1024, 4 * 1024**3)
    tree = RetainedProjectTreePolicy(case_sensitive=True)
    profile_stub = root / PROFILE
    profile_stub.write_text("{}\n")
    members, dirs = inventory(root, roots)
    provisional = RetainedProjectManifest(roots, members, limits, dirs, (), tree)
    tool_members = {m.path: m for m in members if m.role == "toolchain"}

    def tool(name):
        m = tool_members["sdk/bin/" + name]
        return {"name": name, "path": m.path, "identity": "sha256:" + m.sha256}

    worker = ExecutionWorker("local", ExecutionWorkerKind.LOCAL, target_profile=TARGET)
    empty = canonical_identity({"schema": "literate-ai/retained-role-closure@1",
        "roles": ["dependency"], "members": [], "tree_policy": tree.to_dict()})
    # Placeholder only: literate.release.json does not exist in this repository.
    release_placeholder = ContentIdentity.parse_uri("sha256:" + hashlib.sha256(b"").hexdigest())
    policy_member = next(m for m in members if m.path == POLICY)
    profile = {
        "schema": "literate-ai/retained-project-profile@1",
        "authority_mode": "retained-project",
        "target": TARGET,
        "worker_identity": worker.identity.uri,
        "toolchain_identity": provisional.role_identity({"sdk", "toolchain"}).uri,
        "dependency_identity": empty.uri,
        "test_policy_identity": "sha256:" + policy_member.sha256,
        "release_policy_identity": release_placeholder.uri,
        "tools": [tool("python3.12"), tool("make"), tool("true")],
        "actions": [
            {"id": "draft-unknown-build", "stage": "build", "tool": "true",
             "argv": ["sdk/bin/true"], "requires": [],
             "inventory": ["UNKNOWN-per-origin-build-actions"],
             "outputs": ["retained-out/build"], "timeout_seconds": 3600},
            {"id": "draft-original-unit-tests", "stage": "test", "tool": "python3.12",
             "argv": ["sdk/bin/python3.12", "-m", "unittest", "discover", "-s",
                      "Tools/daemonarchy", "-p", "test_*.py"],
             "requires": ["draft-unknown-build"], "inventory": list(cases),
             "outputs": ["retained-out/tests"], "timeout_seconds": 3600},
            {"id": "draft-unknown-docs", "stage": "docs", "tool": "true",
             "argv": ["sdk/bin/true"], "requires": ["draft-unknown-build"],
             "inventory": ["UNKNOWN-docs-actions"],
             "outputs": ["retained-out/docs"], "timeout_seconds": 3600},
            {"id": "draft-unknown-accept", "stage": "accept", "tool": "true",
             "argv": ["sdk/bin/true"],
             "requires": ["draft-unknown-build", "draft-original-unit-tests",
                          "draft-unknown-docs"],
             "inventory": ["UNKNOWN-independent-acceptance"],
             "outputs": ["retained-out/accept"], "timeout_seconds": 3600},
        ],
        "package_inventory": ["retained-out/build"],
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
        "test_cases": len(cases),
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
