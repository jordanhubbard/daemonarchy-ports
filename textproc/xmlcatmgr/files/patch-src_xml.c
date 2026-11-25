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
@@ -311,6 +325,17 @@

     tdata = get_type_info(type);
     if (tdata == NULL)
+        return false;
+    if (tdata->t_params == 0) {
+        warnx("entry type `%s' cannot be added", type);
+        return false;
+    }
+    if (tdata->t_params != (replace == NULL ? 1 : 2)) {
+        warnx("wrong number of arguments for entry type `%s'", type);
+        return false;
+    }
+    if (!valid_attribute_value(orig) ||
+        (replace != NULL && !valid_attribute_value(replace)))
         return false;

     if (search_entry(XMLDOC_ROOT(c), type, orig, false) != NULL) {
@@ -359,6 +384,10 @@
         tdata = get_type_info(type);
         if (tdata == NULL)
             return false;
+        if (tdata->t_params == 0) {
+            warnx("entry type `%s' cannot be removed", type);
+            return false;
+        }
     }

     found = false;
@@ -409,7 +438,8 @@
             XMLNODE_FOREACH_ATTR(aiter, xn) {
                 if (strcmp(XMLATTR_NAME(aiter), tdata->t_origattr) == 0)
                     seenorig = true;
-                else if (strcmp(XMLATTR_NAME(aiter), tdata->t_replattr) == 0)
+                else if (tdata->t_replattr != NULL &&
+                         strcmp(XMLATTR_NAME(aiter), tdata->t_replattr) == 0)
                     seenrepl = true;
                 else if (!seenorig && !seenrepl) {
                     warnx("unknown attribute <%s>", XMLATTR_NAME(aiter));
