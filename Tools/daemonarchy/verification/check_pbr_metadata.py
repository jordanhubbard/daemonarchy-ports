"""Authored metadata/CLI regression controls, separate from PBR's upstream suite."""
import contextlib
import importlib
import io
import json
from pathlib import Path
import sys
import tempfile
import types
import unittest
from unittest import mock

source = Path(sys.argv.pop(1)).resolve()
sys.path.insert(0, str(source))
from pbr._compat import metadata
from pbr.cmd import main
assert Path(metadata.__file__).is_relative_to(source)


class MetadataConsumers(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.site = Path(self.tmp.name)
        self.info = self.site / 'daemonarchy_pbr_metadata_fixture-9.8.7.dist-info'
        self.info.mkdir()
        (self.info / 'METADATA').write_text(
            'Metadata-Version: 2.1\nName: daemonarchy-pbr-metadata-fixture\n'
            'Version: 9.8.7\nPbr-Json: this-header-is-not-the-sidecar\n'
        )
        self.expected = {'git_version': '1a2b3c4', 'is_release': True}
        (self.info / 'pbr.json').write_text(json.dumps(self.expected))
        sys.path.insert(0, str(self.site))
        self.addCleanup(sys.path.remove, str(self.site))
        metadata._metadata_lib = metadata.METADATA_LIB_STDLIB
        self.addCleanup(setattr, metadata, '_metadata_lib', None)
        importlib.invalidate_caches()

    def test_actual_importlib_distribution_name(self):
        import importlib.metadata
        d = importlib.metadata.distribution('daemonarchy-pbr-metadata-fixture')
        self.assertEqual(metadata.get_name(d), 'daemonarchy-pbr-metadata-fixture')

    def test_actual_json_sidecar_not_metadata_header(self):
        self.assertEqual(metadata.get_metadata('daemonarchy-pbr-metadata-fixture'), self.expected)
        info = main._get_info('daemonarchy-pbr-metadata-fixture')
        self.assertEqual(info, {'name': 'daemonarchy-pbr-metadata-fixture', 'version': '9.8.7', 'sha': '1a2b3c4', 'released': 'released'})

    def test_missing_sidecar(self):
        (self.info / 'pbr.json').unlink()
        self.assertIsNone(metadata.get_metadata('daemonarchy-pbr-metadata-fixture'))

    def test_invalid_sidecar(self):
        (self.info / 'pbr.json').write_text('{invalid')
        self.assertIsNone(metadata.get_metadata('daemonarchy-pbr-metadata-fixture'))

    def test_missing_distribution(self):
        with self.assertRaises(metadata.PackageNotFound):
            metadata.get_metadata('daemonarchy-deliberately-missing-pbr-fixture')

    def test_actual_freeze_reports_fixture_and_git_sha(self):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            main.freeze(None)
        self.assertIn('daemonarchy-pbr-metadata-fixture==9.8.7  # git sha 1a2b3c4', out.getvalue().splitlines())

    def test_cli_error_status_and_stderr(self):
        out, err = io.StringIO(), io.StringIO()
        with mock.patch.object(sys, 'argv', ['pbr', 'info', 'daemonarchy-deliberately-missing-pbr-fixture']), contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            status = main.main()
        self.assertEqual(status, 1)
        self.assertEqual(out.getvalue(), '')
        self.assertIn('daemonarchy-deliberately-missing-pbr-fixture', err.getvalue())

    def test_legacy_distribution_name_contract(self):
        metadata._metadata_lib = metadata.METADATA_LIB_LEGACY
        self.assertEqual(metadata.get_name(types.SimpleNamespace(project_name='Legacy-Name')), 'Legacy-Name')

    def test_backport_branch_read_text_contract(self):
        # Exercise the adapter branch against the real stdlib Distribution API.
        # This is not a claim about an installed importlib_metadata backport.
        import importlib.metadata
        metadata._metadata_lib = metadata.METADATA_LIB_BACKPORT
        with mock.patch.dict(sys.modules, {'importlib_metadata': importlib.metadata}):
            self.assertEqual(metadata.get_metadata('daemonarchy-pbr-metadata-fixture'), self.expected)


if __name__ == '__main__':
    unittest.main(verbosity=2)
