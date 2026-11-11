#!/usr/bin/env python3
"""Capture native failing libfprint stacks; never award qualification."""
import json
import os
from pathlib import Path
import signal
import subprocess
import sys

build, output = map(Path, sys.argv[1:])
output.mkdir(parents=True, exist_ok=False)
tests = json.loads(subprocess.check_output(
    ['/usr/local/bin/meson', 'introspect', '--tests', str(build)], text=True))
results = []
for name in ('fp-context', 'fp-device'):
    test = next(row for row in tests if row['name'] == name)
    env = os.environ.copy()
    env.update({key: str(value) for key, value in test['env'].items()})
    env['LD_LIBRARY_PATH'] = str(build/'libfprint') + ':' + env.get('LD_LIBRARY_PATH', '')
    command = ['/usr/local/bin/gdb', '--batch',
               '-ex', 'set pagination off', '-ex', 'set confirm off',
               '-ex', 'set debuginfod enabled off', '-ex', 'run',
               '-ex', 'thread apply all bt', '--args', *test['cmd']]
    interrupted = killed = False
    with (output/(name+'.log')).open('wb') as log:
        process = subprocess.Popen(command, cwd=build, env=env,
                                   stdout=log, stderr=subprocess.STDOUT,
                                   start_new_session=True)
        try:
            process.wait(timeout=8)
        except subprocess.TimeoutExpired:
            interrupted = True
            process.send_signal(signal.SIGINT)
            try:
                process.wait(timeout=10)
            except subprocess.TimeoutExpired:
                killed = True
                os.killpg(process.pid, signal.SIGKILL)
                process.wait(timeout=5)
        finally:
            # GDB normally reaps its inferior; clean up only this diagnostic's
            # session if an inferior survived debugger exit.
            try:
                os.killpg(process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
    results.append({'test': name, 'command': command,
                    'debugger_exit': process.returncode,
                    'debugger_interrupted_after_seconds': 8 if interrupted else None,
                    'diagnostic_killed': killed})
(output/'result.json').write_text(json.dumps({
    'schema': 'daemonarchy/libfprint-native-debug@1',
    'qualification_awarded': False, 'diagnostics': results
}, indent=2)+'\n')
