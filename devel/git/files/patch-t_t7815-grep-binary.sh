FreeBSD's REG_STARTEND implementation lets dot match an embedded NUL.
Require the supported behavior instead of reporting an unexpected success.

--- t/t7815-grep-binary.sh.orig
+++ t/t7815-grep-binary.sh
@@ -63,7 +63,7 @@ test_expect_success 'git grep ile a' '
 	git grep ile a
 '
 
-test_expect_failure !CYGWIN,!MACOS 'git grep .fi a' '
+test_expect_success 'git grep .fi a' '
 	git grep .fi a
 '
 
