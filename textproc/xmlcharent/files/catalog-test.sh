#!/bin/sh
# Exercise PUBLIC-only resolution offline, including character values.
# Keep generated inputs and logs in WRKDIR, including on failure.
set -eu
catalog=$1
entities=$2
work=$3
mkdir "$work"
cp "$catalog" "$work/catalog.xml"
cp "$entities"/*.ent "$work/"
work=$(cd "$work" && pwd)
export XML_CATALOG_FILES="$work/catalog.xml"
unset SGML_CATALOG_FILES
count=0
while IFS='|' read -r public file entity expected; do
    count=$((count + 1))
    resolved=$(xmlcatalog "$XML_CATALOG_FILES" "$public")
    case "$resolved" in
        "$work/$file"|"file://$work/$file") ;;
        *) echo "PUBLIC identifier did not resolve locally: $public" >&2; exit 1 ;;
    esac
    document="$work/probe-$count.xml"
    cat > "$document" <<EOF
<!DOCTYPE value [
<!ELEMENT value (#PCDATA)>
<!ENTITY % chars PUBLIC "$public" "urn:xmlcharent:unmapped:$file">
%chars;
]><value>&$entity;</value>
EOF
    xmllint --nonet --loaddtd --noent --valid "$document" \
        > "$work/probe-$count.stdout" 2> "$work/probe-$count.stderr"
    actual=$(xmllint --xpath 'string(/value)' "$work/probe-$count.stdout")
    test "$actual" = "$expected"
    last_expected=$expected
    echo "PASS: $file PUBLIC resolution and character value"
done <<'CASES'
ISO 8879:1986//ENTITIES Added Latin 1//EN//XML|iso-lat1.ent|aacute|á
ISO 8879:1986//ENTITIES Added Latin 2//EN//XML|iso-lat2.ent|abreve|ă
ISO 8879:1986//ENTITIES Greek Letters//EN//XML|iso-grk1.ent|agr|α
ISO 8879:1986//ENTITIES Monotoniko Greek//EN//XML|iso-grk2.ent|aacgr|ά
ISO 8879:1986//ENTITIES Russian Cyrillic//EN//XML|iso-cyr1.ent|acy|а
ISO 8879:1986//ENTITIES Non-Russian Cyrillic//EN//XML|iso-cyr2.ent|djcy|ђ
ISO 8879:1986//ENTITIES Numeric and Special Graphic//EN//XML|iso-num.ent|half|½
ISO 8879:1986//ENTITIES Diacritical Marks//EN//XML|iso-dia.ent|acute|´
ISO 8879:1986//ENTITIES Publishing//EN//XML|iso-pub.ent|emsp| 
ISO 8879:1986//ENTITIES Box and Line Drawing//EN//XML|iso-box.ent|boxh|─
ISO 8879:1986//ENTITIES General Technical//EN//XML|iso-tech.ent|aleph|ℵ
ISO 8879:1986//ENTITIES Greek Symbols//EN//XML|iso-grk3.ent|alpha|α
ISO 8879:1986//ENTITIES Alternative Greek Symbols//EN//XML|iso-grk4.ent|b.alpha|α
ISO 8879:1986//ENTITIES Added Math Symbols: Ordinary//EN//XML|iso-amso.ent|ang|∠
ISO 8879:1986//ENTITIES Added Math Symbols: Binary Operators//EN//XML|iso-amsb.ent|amalg|∐
ISO 8879:1986//ENTITIES Added Math Symbols: Relations//EN//XML|iso-amsr.ent|ape|≊
ISO 8879:1986//ENTITIES Added Math Symbols: Negated Relations//EN//XML|iso-amsn.ent|gnap|
ISO 8879:1986//ENTITIES Added Math Symbols: Arrow Relations//EN//XML|iso-amsa.ent|cularr|↶
ISO 8879:1986//ENTITIES Added Math Symbols: Delimiters//EN//XML|iso-amsc.ent|rceil|⌉
CASES
test "$count" -eq 19
printf '%s\n' '<catalog xmlns="urn:oasis:names:tc:entity:xmlns:xml:catalog"/>' > "$work/empty.xml"
if XML_CATALOG_FILES="$work/empty.xml" xmllint --nonet --loaddtd --noent --valid "$document" \
    > "$work/missing.stdout" 2> "$work/missing.stderr"; then
    echo 'Missing catalog unexpectedly succeeded' >&2
    exit 1
else
    status=$?
    test "$status" -gt 0 && test "$status" -lt 128
fi
echo 'PASS: missing PUBLIC catalog rejected without a signal'
xmllint --nonet --loaddtd --noent --valid "$document" \
    > "$work/recovery.stdout" 2> "$work/recovery.stderr"
actual=$(xmllint --xpath 'string(/value)' "$work/recovery.stdout")
test "$actual" = "$last_expected"
echo 'PASS: catalog recovery preserves character value'
