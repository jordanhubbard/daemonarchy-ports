--- src/xmldoc.c.orig
+++ src/xmldoc.c
@@ -40,7 +40,7 @@
  * - Unicode support.
  * - DOCTYPE support (as it's now, it just reads its contents without
  *   parsing them).
- * - Entity support.
+ * - DTD and general entity support (attribute character references are decoded).
  */
 
 #include "system.h"
@@ -617,6 +617,99 @@
 
 /* --------------------------------------------------------------------- */
 
+/* Append an XML 1.0 character reference as UTF-8, rejecting invalid values. */
+static bool
+append_attribute_char(struct grstr *gs, unsigned long ch)
+{
+    unsigned char bytes[4];
+    size_t n, i;
+
+    if (!(ch == 9 || ch == 10 || ch == 13 ||
+          (ch >= 0x20 && ch <= 0xd7ff) ||
+          (ch >= 0xe000 && ch <= 0xfffd) ||
+          (ch >= 0x10000 && ch <= 0x10ffff)))
+        return false;
+    if (ch < 0x80) {
+        bytes[0] = ch; n = 1;
+    } else if (ch < 0x800) {
+        bytes[0] = 0xc0 | (ch >> 6);
+        bytes[1] = 0x80 | (ch & 0x3f); n = 2;
+    } else if (ch < 0x10000) {
+        bytes[0] = 0xe0 | (ch >> 12);
+        bytes[1] = 0x80 | ((ch >> 6) & 0x3f);
+        bytes[2] = 0x80 | (ch & 0x3f); n = 3;
+    } else {
+        bytes[0] = 0xf0 | (ch >> 18);
+        bytes[1] = 0x80 | ((ch >> 12) & 0x3f);
+        bytes[2] = 0x80 | ((ch >> 6) & 0x3f);
+        bytes[3] = 0x80 | (ch & 0x3f); n = 4;
+    }
+    for (i = 0; i < n; i++)
+        if (!grstr_append_char(gs, bytes[i]))
+            return false;
+    return true;
+}
+
+/* Decode attribute references once; do not expand DTD or general entities. */
+static char *
+decode_attribute(const char *raw)
+{
+    struct grstr *gs = grstr_new();
+    const unsigned char *p = (const unsigned char *)raw;
+
+    if (gs == NULL)
+        return NULL;
+    while (*p != '\0') {
+        if (*p == '&') {
+            unsigned long ch = 0;
+
+            p++;
+            if (*p == '#') {
+                unsigned long base = 10, digits = 0;
+                p++;
+                if (*p == 'x') { base = 16; p++; }
+                while (*p != ';') {
+                    unsigned long digit;
+                    if (*p >= '0' && *p <= '9') digit = *p - '0';
+                    else if (base == 16 && *p >= 'a' && *p <= 'f') digit = *p - 'a' + 10;
+                    else if (base == 16 && *p >= 'A' && *p <= 'F') digit = *p - 'A' + 10;
+                    else goto invalid;
+                    if (digit >= base || ch > (0x10ffff - digit) / base)
+                        goto invalid;
+                    ch = ch * base + digit; digits++; p++;
+                }
+                if (digits == 0) goto invalid;
+                p++;
+            } else if (strncmp((const char *)p, "amp;", 4) == 0) {
+                ch = '&'; p += 4;
+            } else if (strncmp((const char *)p, "lt;", 3) == 0) {
+                ch = '<'; p += 3;
+            } else if (strncmp((const char *)p, "gt;", 3) == 0) {
+                ch = '>'; p += 3;
+            } else if (strncmp((const char *)p, "quot;", 5) == 0) {
+                ch = '"'; p += 5;
+            } else if (strncmp((const char *)p, "apos;", 5) == 0) {
+                ch = '\''; p += 5;
+            } else goto invalid;
+            if (!append_attribute_char(gs, ch)) goto invalid;
+        } else {
+            unsigned char ch = *p++;
+            if (ch == '<' || (ch < 0x20 && ch != 9 && ch != 10 && ch != 13))
+                goto invalid;
+            if (ch == '\r') {
+                if (*p == '\n') p++;
+                ch = ' ';
+            } else if (ch == '\n' || ch == '\t') ch = ' ';
+            if (!grstr_append_char(gs, ch)) goto invalid;
+        }
+    }
+    return grstr_to_text(gs);
+invalid:
+    warnx("invalid or unsupported XML attribute reference");
+    grstr_free(gs);
+    return NULL;
+}
+
 /*
  * Get an attribute from the file.  We assume that its name has been
  * already read and is passed in the 'name' parameter, so the file should
@@ -640,7 +733,10 @@
             TOKEN_FREE(type, token);
             warnx("unexpected token (quoted string expected)");
         } else {
-            attr = xmlattr_new(name, token);
+            char *value = decode_attribute(token);
+            free(token);
+            if (value != NULL)
+                attr = xmlattr_new(name, value);
         }
     }
 
@@ -797,10 +893,10 @@
     type = TOKEN_STRING;
 
     while (type != TOKEN_ERROR && (ch = fgetc(f)) != EOF && ch != '"')
-        if (!grstr_append_char(gs, ch))
+        if (ch == 0 || !grstr_append_char(gs, ch))
             type = TOKEN_ERROR;
 
-    return ferror(f) ? TOKEN_ERROR : type;
+    return ferror(f) || ch != '"' ? TOKEN_ERROR : type;
 }
 
 /* --------------------------------------------------------------------- */
