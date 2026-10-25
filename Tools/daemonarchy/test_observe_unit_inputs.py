#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
"""Portable producer-observer controls; fake make is not native ports evidence."""
import copy
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

SCRIPT = Path(__file__).with_name("observe-unit-inputs.py")
spec = importlib.util.spec_from_file_location("ports_observer", SCRIPT)
observer = importlib.util.module_from_spec(spec)
spec.loader.exec_module(observer)


class ObserverTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.repo = self.root / "repo"
        self.repo.mkdir()
        self.git("init", "-q")
        self.git("config", "user.email", "test@example.invalid")
        self.git("config", "user.name", "Ports fixture")
        for name, data in (("category/pkg/Makefile", "PORTNAME=fixture\n"),
                           ("category/pkg/distinfo", "SHA256 (fixture.tar) = recorded\n"),
                           ("category/pkg/files/patch-one", "fixture patch\n"),
                           ("Mk/bsd.port.mk", "# shared native make input\n")):
            path = self.repo / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(data)
        self.git("add", ".")
        self.git("commit", "-qm", "fixture")
        self.revision = self.git("rev-parse", "HEAD")
        self.context = {"architecture": "amd64", "osversion": 1600029, "flavor": None,
                        "options": {"FOO": True, "BAR": False}, "make_variables": {"CC": "/usr/bin/cc"}}
        self.values = {name: "" for name in observer.VARIABLES}
        self.values.update(PKGNAME="fixture-1.0", PKGORIGIN="category/pkg", ARCH="amd64", OPSYS="FreeBSD",
                           OSVERSION="1600029", COMPLETE_OPTIONS_LIST="FOO BAR", PORT_OPTIONS="FOO",
                           BUILD_DEPENDS="cmake:devel/cmake", LIB_DEPENDS="libfixture.so:devel/fixture",
                           RUN_DEPENDS="runtime>=1:lang/runtime", TEST_DEPENDS="pytest:devel/pytest",
                           DISTINFO_FILE=str(self.repo/"category/pkg/distinfo"))
        self.values[".MAKE.MAKEFILES"] = "Makefile " + str(self.repo / "Mk/bsd.port.mk")
        self.values_file = self.root / "values.json"
        self.fake = self.root / "fake-make"
        self.fake.write_text("#!" + sys.executable + "\n" +
            "import json,os,sys,time\n" +
            "from pathlib import Path\n" +
            "v=json.loads(Path(" + repr(str(self.values_file)) + ").read_text())\n" +
            "if '__sleep' in v:\n" +
            " Path(" + repr(str(self.root/"child.pid")) + ").write_text(str(os.getpid()))\n" +
            " time.sleep(v['__sleep'])\n" +
            "for i,arg in enumerate(sys.argv):\n" +
            " if arg=='-V': print(v[sys.argv[i+1]])\n")
        self.fake.chmod(0o755)
        self.output = self.root / "evidence"
        self.sentinel = self.root / "existing-package-cache"
        self.sentinel.write_bytes(b"preserve every existing payload")

    def tearDown(self):
        self.temp.cleanup()

    def git(self, *args):
        return subprocess.check_output(["git", "-C", str(self.repo), *args], text=True).strip()

    def observe(self, **kwargs):
        self.values_file.write_text(json.dumps(self.values))
        with patch.object(observer, "MAKE", str(self.fake)), patch.object(observer.platform, "system", return_value="FreeBSD"), \
                patch.object(observer.os, "geteuid", return_value=1000):
            return observer.observe(self.repo, self.revision, "category/pkg", self.context, self.output,
                                    acknowledged=True, **kwargs)

    def test_lib_depends_has_build_and_runtime_edges_and_input_bytes(self):
        report = self.observe()
        library = [e for e in report["dependencies"] if e["declaration"] == "LIB_DEPENDS"]
        self.assertEqual({e["kind"] for e in library}, {"build", "runtime"})
        self.assertEqual({e["origin"] for e in library}, {"devel/fixture"})
        self.assertIn("test", {e["kind"] for e in report["dependencies"]})
        self.assertIn("Mk/bsd.port.mk", {i.get("path") for i in report["inputs"]})
        self.assertIn("category/pkg/distinfo", {i.get("path") for i in report["inputs"]})
        self.assertFalse(report["cache_admission"])
        self.assertFalse(report["recursive_closure_complete"])
        self.assertFalse(report["native_profile_admitted"])
        self.assertEqual(self.git("status", "--porcelain"), "")
        self.assertEqual(self.sentinel.read_bytes(), b"preserve every existing payload")
        request = json.loads((self.output/"request.json").read_bytes())
        self.assertIn("OPTIONS_OVERRIDE=FOO", request["argv"])
        for variable in ("WRKDIRPREFIX", "DISTDIR", "PORT_DBDIR", "PKG_DBDIR", "PACKAGES", "MAKEOBJDIRPREFIX"):
            selected = [a.split("=", 1)[1] for a in request["argv"] if a.startswith(variable + "=")]
            self.assertEqual(len(selected), 1)
            self.assertTrue(Path(selected[0]).is_relative_to(self.output))
        self.assertEqual((self.output/"make.exit").read_text(), "0\n")

    def test_existing_evidence_is_never_overwritten(self):
        self.observe()
        original = (self.output/"observation.json").read_bytes()
        with self.assertRaises(FileExistsError):
            self.observe()
        self.assertEqual((self.output/"observation.json").read_bytes(), original)

    def test_incomplete_and_changed_options_are_retained_failures(self):
        self.values["COMPLETE_OPTIONS_LIST"] += " MISSING"
        with self.assertRaisesRegex(ValueError, "options inventory is incomplete"):
            self.observe()
        self.assertTrue((self.output/"make.stdout").exists())
        self.assertFalse(json.loads((self.output/"failure.json").read_text())["cache_admission"])

    def test_wrong_flavor_does_not_silently_select_default(self):
        self.values["FLAVOR"] = "py313"
        with self.assertRaisesRegex(ValueError, "native flavor"):
            self.observe()

    def test_target_context_is_exact(self):
        self.values["OSVERSION"] = "1600030"
        with self.assertRaisesRegex(ValueError, "target/ABI"):
            self.observe()

    def test_missing_dynamic_dependency_expression_refuses(self):
        self.values["BUILD_DEPENDS"] = "tool:${MISSING_ORIGIN}"
        with self.assertRaisesRegex(ValueError, "unresolved native dependency"):
            self.observe()

    def test_shared_input_mutation_refuses_before_evaluation(self):
        (self.repo/"Mk/bsd.port.mk").write_text("changed shared input")
        with self.assertRaises(subprocess.CalledProcessError):
            self.observe()
        self.assertFalse(self.output.exists())

    def test_generated_package_file_is_an_explicit_gap(self):
        self.values["PLIST"] = str(self.root/"not-generated-yet")
        report = self.observe()
        self.assertEqual(report["unresolved_inputs"][0]["variable"], "PLIST")
        self.assertFalse(report["cache_admission"])

    def test_master_port_inputs_are_inventoried(self):
        shared = self.repo/"category/master/files/patch-shared"
        shared.parent.mkdir(parents=True)
        shared.write_text("master patch bytes")
        self.git("add", ".")
        self.git("commit", "-qm", "master")
        self.revision = self.git("rev-parse", "HEAD")
        self.values["MASTERDIR"] = str(self.repo/"category/master")
        report = self.observe()
        self.assertIn("category/master/files/patch-shared", {i.get("path") for i in report["inputs"]})

    def test_deadline_reaps_own_process_and_preserves_partial_evidence(self):
        self.values["__sleep"] = 10
        with self.assertRaisesRegex(TimeoutError, "deadline"):
            self.observe(timeout=1)
        pid = int((self.root/"child.pid").read_text())
        with self.assertRaises(ProcessLookupError):
            os.kill(pid, 0)
        self.assertTrue((self.output/"make.exit").exists())
        self.assertTrue((self.output/"failure.json").exists())
        self.assertEqual(self.sentinel.read_bytes(), b"preserve every existing payload")

    def test_context_cannot_override_isolation_or_expand_shell(self):
        invalid = copy.deepcopy(self.context)
        invalid["make_variables"]["DISTDIR"] = "/existing/cache"
        with self.assertRaises(ValueError):
            observer.validate_context(invalid)
        invalid = copy.deepcopy(self.context)
        invalid["make_variables"]["CC"] = "$(unreviewed-expansion)"
        with self.assertRaises(ValueError):
            observer.validate_context(invalid)

    def test_acknowledgement_and_native_worker_are_required(self):
        with self.assertRaisesRegex(ValueError, "acknowledge"):
            observer.observe(self.repo, self.revision, "category/pkg", self.context, self.output,
                             acknowledged=False)
        with patch.object(observer.platform, "system", return_value="Linux"):
            with self.assertRaisesRegex(ValueError, "native FreeBSD"):
                observer.observe(self.repo, self.revision, "category/pkg", self.context, self.output,
                                 acknowledged=True)
        self.assertFalse(self.output.exists())

    def test_flavored_dependency_and_target_are_preserved(self):
        values = {name: "" for name in observer.DEPENDENCIES}
        values["BUILD_DEPENDS"] = "python:lang/python@py313:build"
        edges = observer.dependency_edges(values)
        self.assertEqual(edges[0]["origin"], "lang/python@py313")
        self.assertEqual(edges[0]["target"], "build")


if __name__ == "__main__":
    unittest.main()
