--- tests/machine_api.rs.orig
+++ tests/machine_api.rs
@@ -22,6 +22,10 @@
     '/bin/sh -c '*)
         if [ "$TEST_MODE" = offline ]; then echo 'test remote connection failed' >&2; exit 255; fi
         printf 'login banner\n'
+        # The fake SSH host contains only the harness-controlled installation.
+        # Do not let discovery fall through to an ambient host package binary.
+        last=$(printf '%s\n' "$last" |
+            sed 's|emit "/[^"]*"|emit "$TEST_ROOT/remote bin/herdr"|g')
         PATH="$TEST_ROOT/remote bin:/usr/bin:/bin" exec /bin/sh -c "$last" ;;
     '/bin/sh -s')
         script=$(cat)
