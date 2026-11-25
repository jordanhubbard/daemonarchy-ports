--- src/xmlnode.c.orig
+++ src/xmlnode.c
@@ -94,7 +94,30 @@
 {
     assert(xa != NULL && f != NULL);
 
-    return fprintf(f, "%s=\"%s\"", xa->xa_name, xa->xa_value) != -1;
+    const unsigned char *p;
+
+    if (fprintf(f, "%s=\"", xa->xa_name) < 0)
+        return false;
+    for (p = (const unsigned char *)xa->xa_value; *p != '\0'; p++) {
+        const char *escaped = NULL;
+
+        switch (*p) {
+        case '&': escaped = "&amp;"; break;
+        case '<': escaped = "&lt;"; break;
+        case '>': escaped = "&gt;"; break;
+        case '"': escaped = "&quot;"; break;
+        case '\'': escaped = "&apos;"; break;
+        case '\t': escaped = "&#9;"; break;
+        case '\n': escaped = "&#10;"; break;
+        case '\r': escaped = "&#13;"; break;
+        }
+        if (escaped != NULL) {
+            if (fputs(escaped, f) == EOF)
+                return false;
+        } else if (fputc(*p, f) == EOF)
+            return false;
+    }
+    return fputc('"', f) != EOF;
 }
 
 /* --------------------------------------------------------------------- */
