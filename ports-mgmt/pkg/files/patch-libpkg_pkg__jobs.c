--- libpkg/pkg_jobs.c.orig
+++ libpkg/pkg_jobs.c
@@ -1009,6 +1009,20 @@
 				    "%s is not installed, therefore upgrade is impossible",
 				    pkg->name);
 				return (EPKG_NOTINSTALLED);
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
 			}
 			pkg->type = PKG_FILE;
 			pkg_jobs_add_req(j, pkg);
