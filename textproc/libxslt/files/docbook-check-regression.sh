#!/bin/sh
# Exercise the real DocBook recipes, including their failure status.
# Keep every trial and log; do not modify the original source or its goldens.
set -eu
source=$1
make=$2
work=$3
trial=$(mktemp -d "$work/docbook-check-regression.XXXXXXXX")
cp -Rp "$source/tests/docbook" "$trial/tree"
mkdir "$trial/logs"

run_case()
{
    name=$1
    expected=$2
    shift 2
    if "$make" --no-print-directory -C "$trial/tree" \
        "top_builddir=$source" "top_srcdir=$source" srcdir=. "$@" \
        >"$trial/logs/$name.log" 2>&1; then
        status=0
    else
        status=$?
    fi
    if { [ "$expected" = pass ] && [ "$status" -ne 0 ]; } || \
       { [ "$expected" = fail ] && [ "$status" -eq 0 ]; }; then
        cat "$trial/logs/$name.log"
        echo "FAIL: $name returned $status (expected $expected)" >&2
        exit 1
    fi
    if [ "$expected" = fail ]; then
        grep -q DAEMONARCHY_DOCBOOK_FAILURE_CONTROL "$trial/logs/$name.log" || {
            cat "$trial/logs/$name.log"
            echo "FAIL: $name failed without reaching its control" >&2
            exit 1
        }
    fi
    echo "PASS: docbook-check-$name"
}

run_case original-results pass single xtchunk
html=result/html/gdp-handbook.html
printf '\nDAEMONARCHY_DOCBOOK_FAILURE_CONTROL\n' >>"$trial/tree/$html"
run_case single-comparison fail single
cp -p "$source/tests/docbook/$html" "$trial/tree/$html"
run_case single-recovery pass single

chunk=result/xtchunk/html/indexs02.orig
printf '\nDAEMONARCHY_DOCBOOK_FAILURE_CONTROL\n' >>"$trial/tree/$chunk"
run_case chunk-comparison fail xtchunk
cp -p "$source/tests/docbook/$chunk" "$trial/tree/$chunk"
run_case chunk-recovery pass xtchunk

cat >"$trial/reject-generator" <<'EOF'
#!/bin/sh
# Single output is redirected to a file; chunk diagnostics use stderr files.
echo DAEMONARCHY_DOCBOOK_FAILURE_CONTROL
echo DAEMONARCHY_DOCBOOK_FAILURE_CONTROL >&2
exit 42
EOF
chmod 755 "$trial/reject-generator"
run_case single-generator fail single "CHECKER=$trial/reject-generator"
run_case chunk-generator fail xtchunk "CHECKER=$trial/reject-generator"
run_case final-recovery pass single xtchunk
