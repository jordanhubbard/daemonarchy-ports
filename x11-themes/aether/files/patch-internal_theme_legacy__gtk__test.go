--- internal/theme/legacy_gtk_test.go.orig
+++ internal/theme/legacy_gtk_test.go
@@ -11,2 +11,2 @@
-	if runtime.GOOS != "linux" {
-		t.Skip("GTK stylesheets were installed on Linux only")
+	if runtime.GOOS != "linux" && runtime.GOOS != "freebsd" {
+		t.Skip("GTK stylesheets were installed on Linux and FreeBSD only")
