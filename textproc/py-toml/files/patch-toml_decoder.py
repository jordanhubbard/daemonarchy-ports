--- toml/decoder.py.orig
+++ toml/decoder.py
@@ -360,6 +360,9 @@
                               " Reached end of file.", original, len(s))
     if openstring:  # reached EOF and have an unterminated string
         raise TomlDecodeError("Unterminated string found."
+                              " Reached end of file.", original, len(s))
+    if openarr:
+        raise TomlDecodeError("Unbalanced array found."
                               " Reached end of file.", original, len(s))
     s = ''.join(sl)
     s = s.split('\n')
