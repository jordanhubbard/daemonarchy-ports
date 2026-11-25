--- libpkg/pkg_jobs.c.orig
+++ libpkg/pkg_jobs.c
@@ -1011,0 +1012,14 @@
+			}
+			/* File candidates need the installed package in the universe too. */
+			if (j->type == PKG_JOBS_INSTALL || j->type == PKG_JOBS_UPGRADE) {
+				struct pkg *lp = pkg_jobs_universe_get_local(j->universe,
+				    pkg->uid, 0);
+				if (lp != NULL) {
+					rc = pkg_jobs_universe_process_item(j->universe, lp, NULL);
+					if (rc != EPKG_OK) {
+						if (!pkg_in_universe(j->universe, lp))
+							pkg_free(lp);
+						pkg_free(pkg);
+						return (rc);
+					}
+				}
