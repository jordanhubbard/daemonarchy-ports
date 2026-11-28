--- typogrify/templatetags/jinja_filters.py.orig
+++ typogrify/templatetags/jinja_filters.py
@@ -22,6 +22,16 @@
     A function wrapper to make typogrify play nice with jinja2's
     unicode support.
 
+    >>> def fails(text):
+    ...     raise TypogrifyError("filter failed")
+    >>> try:
+    ...     make_safe(fails)("value")
+    ... except TemplateError as error:
+    ...     print(str(error))
+    filter failed
+    >>> isinstance(make_safe(lambda text: text)("native"), Markup)
+    True
+
     """
 
     @wraps(f)
@@ -31,7 +41,7 @@
         try:
             out = f(text)
         except TypogrifyError as e:
-            raise TemplateError(e.message)
+            raise TemplateError(str(e))
         return Markup(out)
 
     wrapper.is_safe = True
