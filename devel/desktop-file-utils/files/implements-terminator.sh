#!/bin/bash
set -euo pipefail

validator="${BUILD_ROOT:?}/src/desktop-file-validate"
installer="$BUILD_ROOT/src/desktop-file-install"
work=$(mktemp -d "${TMPDIR:-/tmp}/desktop-implements.XXXXXX")
failures=0
echo "Implements fixtures and command logs: $work"

check_list() {
  local name=$1 expected=$2 value=$3 status tool
  local before=$failures
  local input="$work/$name.desktop" destination="$work/$name-installed"
  mkdir "$destination"
  printf '[Desktop Entry]\nType=Application\nName=Interface probe\nExec=/usr/bin/true\nImplements=%s\n' "$value" >"$input"
  for tool in validator installer; do
    status=0
    if [[ $tool == validator ]]; then
      "$validator" "$input" >"$work/$name.$tool.log" 2>&1 || status=$?
    else
      "$installer" --dir="$destination" "$input" >"$work/$name.$tool.log" 2>&1 || status=$?
    fi
    if [[ $expected == valid ]]; then
      if (( status != 0 )); then
        echo "not ok - $name: $tool rejected a valid interface list"
        cat "$work/$name.$tool.log"
        failures=$((failures + 1))
      fi
    elif (( status == 0 )) || ! grep -Fq 'invalid interface name' "$work/$name.$tool.log"; then
      echo "not ok - $name: $tool did not diagnose the invalid interface"
      cat "$work/$name.$tool.log"
      failures=$((failures + 1))
    fi
  done
  if [[ $expected == valid ]]; then
    if [[ ! -f $destination/$name.desktop ]]; then
      echo "not ok - $name: no installed desktop entry"
      failures=$((failures + 1))
    elif ! "$validator" "$destination/$name.desktop" >"$work/$name.installed.log" 2>&1; then
      echo "not ok - $name: installed entry failed validation"
      cat "$work/$name.installed.log"
      failures=$((failures + 1))
    fi
  fi
  if (( failures == before )); then
    echo "ok - $name"
  fi
}

check_list empty valid ''
check_list plain valid 'org.freedesktop.Terminal'
check_list terminated valid 'org.freedesktop.Terminal;'
check_list multiple-terminated valid 'org.freedesktop.Terminal;org.freedesktop.UriHandler;'
check_list interior-empty invalid 'org.freedesktop.Terminal;;org.freedesktop.UriHandler'
check_list trailing-empty invalid 'org.freedesktop.Terminal;;'
check_list leading-empty invalid ';org.freedesktop.Terminal'
check_list invalid-interface invalid 'org.freedesktop.Terminal;not_an_interface;'
(( failures == 0 ))
