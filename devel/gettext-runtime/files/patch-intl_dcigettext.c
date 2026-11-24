Keep portable libintl cache entries distinct when the effective LANGUAGE
preference list changes without a catalog-counter update. Preserve the
existing category and per-thread locale keys and the libc build path.

--- intl/dcigettext.c.orig
+++ intl/dcigettext.c
@@ -198,0 +199,5 @@
+#ifndef _LIBC
+  /* Effective language preference list, including LANGUAGE overrides.  */
+  const char *language;
+#endif
+
@@ -250,3 +255 @@
-	    /* We compare the category last (though this is the cheapest
-	       operation) since it is hopefully always the same (namely
-	       LC_MESSAGES).  */
+	    /* Compare the category once the domain and locale match.  */
@@ -255,0 +259,5 @@
+
+#ifndef _LIBC
+  if (result == 0)
+    result = strcmp (s1->language, s2->language);
+#endif
@@ -474,0 +483,9 @@
+#ifndef _LIBC
+  /* A system setlocale() need not invalidate libintl's catalog counter.
+     Include the effective language preferences in the lookup key so that
+     a changed LANGUAGE cannot reuse a translation from an earlier list.  */
+  categoryname = category_to_name (category);
+# define CATEGORYNAME_INITIALIZED
+  categoryvalue = guess_category_value (category, categoryname);
+  search.language = categoryvalue;
+#endif
@@ -479,2 +495,0 @@
-  categoryname = category_to_name (category);
-#  define CATEGORYNAME_INITIALIZED
@@ -635,0 +651 @@
+#ifdef _LIBC
@@ -636,0 +653 @@
+#endif
@@ -744,0 +762,3 @@
+#ifndef _LIBC
+		  size += strlen (search.language) + 1;
+#endif
@@ -751,0 +772,3 @@
+#ifndef _LIBC
+		      char *new_language;
+#endif
@@ -759,0 +783,5 @@
+#ifndef _LIBC
+		      new_language = new_domainname + domainname_len + 1;
+		      strcpy (new_language, search.language);
+		      newp->language = new_language;
+#endif
@@ -760,0 +789 @@
+# ifdef _LIBC
@@ -761,0 +791,3 @@
+# else
+		      new_localename = new_language + strlen (search.language) + 1;
+# endif
