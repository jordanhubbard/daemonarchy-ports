Preserve the XML grammar while supporting SGML mode in the shared modules.
Backport the later DocBook SGML guards and tag-minimization handling; keep
XML 4.3 rules as an enumeration and use CDATA only for SGML token conflicts.

--- docbook-4.3/dbpoolx.mod.orig
+++ docbook-4.3/dbpoolx.mod
@@ -635,0 +636,3 @@
+<![%sgml.features;[
+<!ENTITY % xml-base.attrib "">
+]]>
