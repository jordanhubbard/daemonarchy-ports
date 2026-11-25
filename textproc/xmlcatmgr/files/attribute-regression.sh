#!/bin/sh
# Exercise the real binary; retain every catalog and command diagnostic.
set -eu
cli=$1
trial=$(mktemp -d "$2/xml-attribute-regression.XXXXXXXX")
catfile=$trial/catalog.xml
fail()
{
    echo "FAIL: xml-attribute-$1" >&2
    exit 1
}
pass()
{
    echo "PASS: xml-attribute-$1"
}
must_fail()
{
    label=$1
    shift
    if "$@" >"$trial/$label.log" 2>&1; then
        fail "$label-unexpected-success"
    fi
}
"$cli" -c "$catfile" create
name='urn:native:query?a=1&b=2'
value='file:///tmp/native.dtd?a=3&b=4'
"$cli" -c "$catfile" add uri "$name" "$value"
grep -Fq 'name="urn:native:query?a=1&amp;b=2"' "$catfile" || fail query-serialization
grep -Fq 'uri="file:///tmp/native.dtd?a=3&amp;b=4"' "$catfile" || fail target-serialization
"$cli" -c "$catfile" lookup "$name" >"$trial/query-lookup.log"
"$cli" -c "$catfile" remove uri "$name"
must_fail removed-query "$cli" -c "$catfile" lookup "$name"
pass query-roundtrip

name='urn:native:literal:&amp;'
"$cli" -c "$catfile" add uri "$name" file:///tmp/literal
grep -Fq 'name="urn:native:literal:&amp;amp;"' "$catfile" || fail double-escaping
"$cli" -c "$catfile" lookup "$name" >"$trial/literal-lookup.log"
"$cli" -c "$catfile" remove uri "$name"
pass single-entity-decode

cat >"$catfile" <<'EOF'
<?xml version="1.0"?>
<catalog xmlns="urn:oasis:names:tc:entity:xmlns:xml:catalog">
<uri name="urn:native:&amp;&lt;&gt;&quot;&apos;" uri="file:///tmp/predefined" />
<uri name="urn:native:&#65;&#x42;&#x3BB;&#x1F642;" uri="file:///tmp/numeric" />
</catalog>
EOF
name="urn:native:&<>\"'"
"$cli" -c "$catfile" lookup "$name" >"$trial/predefined-lookup.log"
"$cli" -c "$catfile" remove uri "$name"
pass predefined-references
name="urn:native:AB$(printf '\316\273\360\237\231\202')"
"$cli" -c "$catfile" lookup "$name" >"$trial/numeric-lookup.log"
"$cli" -c "$catfile" remove uri "$name"
pass numeric-Unicode-references

cat >"$catfile" <<'EOF'
<?xml version="1.0"?>
<catalog xmlns="urn:oasis:names:tc:entity:xmlns:xml:catalog">
<uri name="urn:native:literal
	 space" uri="file:///tmp/literal-space" />
<uri name="urn:native:reference&#9;&#10;&#13;end" uri="file:///tmp/reference-space" />
</catalog>
EOF
"$cli" -c "$catfile" lookup 'urn:native:literal   space' >"$trial/literal-space.log"
name="urn:native:reference$(printf '\011\012\015')end"
"$cli" -c "$catfile" lookup "$name" >"$trial/reference-space.log"
"$cli" -c "$catfile" add uri urn:native:unrelated file:///tmp/unrelated
"$cli" -c "$catfile" lookup "$name" >"$trial/reference-after-rewrite.log"
grep -Fq 'name="urn:native:reference&#9;&#10;&#13;end"' "$catfile" || fail referenced-whitespace
pass attribute-whitespace

n=0
for ref in '&unknown;' '&#0;' '&#xD800;' '&#x110000;' '&#;' '&#x;' '&#999999999999999999999999;' '&amp'; do
    n=$((n + 1))
    bad=$trial/invalid-$n.xml
    printf '<?xml version="1.0"?>\n<catalog xmlns="urn:oasis:names:tc:entity:xmlns:xml:catalog"><uri name="%s" uri="file:///tmp/bad" /></catalog>\n' "$ref" >"$bad"
    cp -p "$bad" "$bad.before"
    must_fail invalid-reference-$n "$cli" -c "$bad" lookup whatever
    cmp -s "$bad" "$bad.before" || fail invalid-reference-mutated-input
 done
pass invalid-references

cp -p "$catfile" "$trial/before-control.xml"
control="urn:native:bad$(printf '\001')"
must_fail invalid-control "$cli" -c "$catfile" add uri "$control" file:///tmp/bad
cmp -s "$catfile" "$trial/before-control.xml" || fail rejected-control-mutated-catalog
"$cli" -c "$catfile" add uri urn:native:recovery file:///tmp/recovery
"$cli" -c "$catfile" lookup urn:native:recovery >"$trial/recovery.log"
pass rejection-and-recovery

sgml=$trial/sgml.catalog
"$cli" -s -c "$sgml" create
"$cli" -s -c "$sgml" add SYSTEM 'urn:native:sgml?a=1&b=2' 'file:///tmp/sgml?a=3&b=4'
"$cli" -s -c "$sgml" lookup 'urn:native:sgml?a=1&b=2' >"$trial/sgml-lookup.log"
grep -Fq 'SYSTEM "urn:native:sgml?a=1&b=2" "file:///tmp/sgml?a=3&b=4"' "$sgml" || fail SGML-changed
"$cli" -s -c "$sgml" remove SYSTEM 'urn:native:sgml?a=1&b=2'
pass SGML-unchanged
