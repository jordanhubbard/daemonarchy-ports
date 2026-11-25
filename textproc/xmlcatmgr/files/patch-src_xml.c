--- src/xml.c.orig
+++ src/xml.c
@@ -294,6 +294,20 @@
 
 /* --------------------------------------------------------------------- */
 
+static bool
+valid_attribute_value(const char *value)
+{
+    const unsigned char *p;
+
+    for (p = (const unsigned char *)value; *p != '\0'; p++) {
+        if (*p < 0x20 && *p != 9 && *p != 10 && *p != 13) {
+            warnx("invalid XML attribute control character");
+            return false;
+        }
+    }
+    return true;
+}
+
 /*
  * Adds the given entry to the catalog file, if it's not already present.
  * Also checks if the type is valid.  'replace' may be null if the given
@@ -311,6 +325,9 @@
 
     tdata = get_type_info(type);
     if (tdata == NULL)
+        return false;
+    if (!valid_attribute_value(orig) ||
+        (replace != NULL && !valid_attribute_value(replace)))
         return false;
 
     if (search_entry(XMLDOC_ROOT(c), type, orig, false) != NULL) {
