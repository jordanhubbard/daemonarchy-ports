--- internal/theme/applier_test.go.orig
+++ internal/theme/applier_test.go
@@ -11,2 +11,2 @@
-	if runtime.GOOS != "linux" {
-		t.Skip("Omarchy is only supported on Linux")
+	if runtime.GOOS != "linux" && runtime.GOOS != "freebsd" {
+		t.Skip("Omarchy is only supported on Linux and FreeBSD")
