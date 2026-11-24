GNU libintl redirects these public functions to prefixed symbols on
FreeBSD. Use its supported macro redirection so both the interposed
entry points and RTLD_NEXT lookup names use the same symbols.

--- bindtextdomain.c.orig
+++ bindtextdomain.c
@@ -23,0 +24,6 @@
+#define _INTL_REDIRECT_MACROS 1
+#include <libintl.h>
+
+/* Resolve the same public symbols selected by libintl.h. */
+#define SYMBOL_NAME_1(name) #name
+#define SYMBOL_NAME(name) SYMBOL_NAME_1(name)
@@ -51 +57 @@
-    if (!(r_textdomain = dlsym(RTLD_NEXT, "textdomain")))
+    if (!(r_textdomain = dlsym(RTLD_NEXT, SYMBOL_NAME(textdomain))))
@@ -54 +60 @@
-    if (!(r_bindtextdomain = dlsym(RTLD_NEXT, "bindtextdomain")))
+    if (!(r_bindtextdomain = dlsym(RTLD_NEXT, SYMBOL_NAME(bindtextdomain))))
@@ -58 +64 @@
-    					    "bind_textdomain_codeset")))
+    					    SYMBOL_NAME(bind_textdomain_codeset))))
