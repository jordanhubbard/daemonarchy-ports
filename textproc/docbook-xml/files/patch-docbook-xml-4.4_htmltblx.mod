Preserve the XML grammar while supporting SGML mode in the shared modules.
Backport the later DocBook SGML guards and tag-minimization handling; keep
XML 4.3 rules as an enumeration and use CDATA only for SGML token conflicts.

--- docbook-xml-4.4/htmltblx.mod.orig
+++ docbook-xml-4.4/htmltblx.mod
@@ -65,0 +66,3 @@
+<![%sgml.features;[
+<!ENTITY % i18n "">
+]]>
@@ -95,5 +98,5 @@
-<!ELEMENT colgroup (col)*>
-<!ELEMENT col      EMPTY>
-<!ELEMENT tr       (th|td)+>
-<!ELEMENT th       (%para.char.mix; | %tabentry.mix; | table | informaltable)*>
-<!ELEMENT td       (%para.char.mix; | %tabentry.mix; | table | informaltable)*>
+<!ELEMENT colgroup %ho; (col)*>
+<!ELEMENT col %ho; EMPTY>
+<!ELEMENT tr %ho; (th|td)+>
+<!ELEMENT th %ho; (%para.char.mix; | %tabentry.mix; | table | informaltable)*>
+<!ELEMENT td %ho; (%para.char.mix; | %tabentry.mix; | table | informaltable)*>
