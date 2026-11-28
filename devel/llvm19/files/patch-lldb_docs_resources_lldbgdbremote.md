--- lldb/docs/resources/lldbgdbremote.md.orig
+++ lldb/docs/resources/lldbgdbremote.md
@@ -1361,8 +1361,10 @@
                           the start address of the range that contains `<addr>`
 * `size:<size>;` - `<size>` is a big endian hex byte size of the address
                    of the range that contains `<addr>`
-* `permissions:<permissions>;` - `<permissions>` is a string that contains one
-                                 or more of the characters from `rwx`
+* `permissions:<permissions>;` - `<permissions>` is a string containing zero or
+                                 more of the characters from `rwx`. An empty
+                                 value describes a mapped range with no access
+                                 permissions.
 * `name:<name>;` - `<name>` is a hex encoded string that contains the name of
                    the memory region mapped at the given address. In case of
                    regions backed by a file it have to be the absolute path of
@@ -1400,8 +1402,9 @@
   start:2;size:fffffffe;
 ```
 
-The lack of `permissions:` indicates that none of read/write/execute are valid
-for this region.
+A mapped region must include `permissions:`, even when its value is empty.
+The absence of this tuple identifies an unmapped range. An empty value
+(`permissions:;`) identifies mapped memory without read, write or execute access.
 
 **Priority To Implement:** Medium
 
