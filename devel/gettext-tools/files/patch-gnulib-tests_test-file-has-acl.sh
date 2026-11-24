FreeBSD's setfacl requires complete entries with -x and recalculates a
mask by default.  Once the named user is gone, remove the remaining
extended ACL without recreating its mask.  Keep the original assertions.

--- gnulib-tests/test-file-has-acl.sh.orig
+++ gnulib-tests/test-file-has-acl.sh
@@ -213,6 +213,7 @@ func_tmpdir
           # Remove the ACL for the mask, if it was implicitly added.
           case $acl_flavor in
-            linux | freebsd) setfacl -x mask: tmpfile0 ;;
+            linux)   setfacl -x mask: tmpfile0 ;;
+            freebsd) setfacl -bn tmpfile0 ;;
             *)               setfacl -d mask: tmpfile0 ;;
           esac
 
