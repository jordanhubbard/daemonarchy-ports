#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
"""The omarchy network panel must not trust a stale remote-debug status file.

The panel parser is QML-embedded JavaScript, added by the port's patch. This
lifts updateDebugStatus out of the patch and runs it under node with a fixed
clock. It needs node; without it the tests are skipped, not passed.
"""
import json
from pathlib import Path
import shutil
import subprocess
import unittest

PATCH = Path(__file__).resolve().parents[2] / "x11-wm/omarchy/files/patch-shell_plugins_panels_network_Panel.qml"
NODE = shutil.which("node")
NOW = 1_800_000_000


def parser_source():
    added = [line[1:] for line in PATCH.read_text().splitlines() if line.startswith("+") and not line.startswith("+++")]
    out, inside = [], False
    for line in added:
        if line.startswith("  readonly property int debugStatusMaxAge:"):
            out.append("var debugStatusMaxAge = " + line.split(":", 1)[1].strip())
        if line.startswith("  function updateDebugStatus("):
            inside = True
        if inside:
            out.append(line)
            if line == "  }":
                break
    return "\n".join(out)


@unittest.skipUnless(NODE, "node is required")
class DebugStatusStaleness(unittest.TestCase):
    def run_panel(self, content):
        script = (
            "var debugStatus;\nDate.now = function() { return %d * 1000 }\n%s\n"
            "updateDebugStatus(%s);\nconsole.log(JSON.stringify(debugStatus))" % (NOW, parser_source(), json.dumps(content))
        )
        result = subprocess.run([NODE, "-e", script], capture_output=True, text=True, check=True)
        return json.loads(result.stdout)

    @staticmethod
    def status(grant=1, listener=1, connected=0, updated=NOW, state="listening", legacy=False):
        text = "schema=1\ngrant=%d\nlistener=%d\nconnected=%d\naddresses=10.0.0.2\nport=2222\n" % (grant, listener, connected)
        if not legacy:
            text += "state=%s\nupdated=%s\n" % (state, updated)
        return text

    def test_fresh_status_is_trusted(self):
        for age in (0, 10, 60):
            got = self.run_panel(self.status(updated=NOW - age))
            self.assertFalse(got["unavailable"], age)
            self.assertTrue(got["grant"])
            self.assertTrue(got["listener"])

    def test_fresh_unpaired_is_off_not_unavailable(self):
        got = self.run_panel(self.status(grant=0, listener=0, state="unpaired"))
        self.assertFalse(got["unavailable"])
        self.assertFalse(got["grant"])

    def test_stale_status_is_unavailable_with_no_actions(self):
        for stale in (self.status(updated=NOW - 61), self.status(grant=0, listener=0, updated=NOW - 3600)):
            got = self.run_panel(stale)
            self.assertTrue(got["unavailable"])
            self.assertFalse(got["grant"])
            self.assertFalse(got["listener"])
            self.assertFalse(got["connected"])

    def test_clock_far_in_the_future_is_unavailable(self):
        self.assertTrue(self.run_panel(self.status(updated=NOW + 3600))["unavailable"])

    def test_missing_or_unparseable_is_unavailable(self):
        for content in ("", "garbage", "schema=2\ngrant=1\n", self.status(updated="soon"), self.status(updated=""),
                        self.status(updated="-5"), self.status(updated="1e3")):
            self.assertTrue(self.run_panel(content)["unavailable"], content)

    def test_legacy_status_without_updated_is_trusted(self):
        got = self.run_panel(self.status(legacy=True))
        self.assertFalse(got["unavailable"])
        self.assertTrue(got["grant"])
        self.assertEqual(got["port"], "2222")

    def test_patch_gates_pair_button_on_availability(self):
        text = PATCH.read_text()
        self.assertIn("visible: !root.debugStatus.unavailable && !root.debugStatus.grant", text)
        self.assertIn("status unavailable", text)


if __name__ == "__main__":
    unittest.main()
