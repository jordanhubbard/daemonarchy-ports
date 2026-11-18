#!/bin/sh
# Restore original upstream test inputs omitted from the release tarball.
# Existing files include downstream patches and must never be overwritten.
set -eu

if [ "$#" -ne 3 ]; then
	echo "usage: $0 release-source tag-source xunit-source" >&2
	exit 64
fi
release_source=$1
tag_source=$2
xunit_source=$3

copy_missing_file()
{
	if [ -e "$2" ] || [ -L "$2" ]; then
		return 0
	fi
	mkdir -p "${2%/*}"
	cp -RnP "$1" "$2"
}

restore_tree()
{
	find "$1" \( -type f -o -type l \) -print |
	while IFS= read -r source; do
		copy_missing_file "$source" "$2/${source#"$1/"}" || exit 1
	done
}

for tree in mono/tests mcs/tools/nunit-lite mcs/class/test-helpers; do
	restore_tree "$tag_source/$tree" "$release_source/$tree"
done

find "$tag_source/mcs" -type d -name Test -prune -print |
while IFS= read -r source; do
	relative=${source#"$tag_source/"}
	restore_tree "$source" "$release_source/$relative" || exit 1
done

for tree in mono/mini mono/unit-tests; do
	for source in "$tag_source/$tree/"*.cs "$tag_source/$tree/"*.il \
	    "$tag_source/$tree/"*.snk; do
		[ -f "$source" ] || continue
		copy_missing_file "$source" "$release_source/$tree/${source##*/}"
	done
done

composition=mcs/class/System.ComponentModel.Composition.4.5/corefx
copy_missing_file "$tag_source/$composition/SR.cs" \
    "$release_source/$composition/SR.cs"
for source in "$xunit_source/"*.dll "$xunit_source/"*.exe; do
	copy_missing_file "$source" \
	    "$release_source/external/xunit-binaries/${source##*/}"
done
