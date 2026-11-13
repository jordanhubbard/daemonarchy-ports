--- libpkg/pkg_jobs_universe.c.orig
+++ libpkg/pkg_jobs_universe.c
@@ -287,8 +287,20 @@
 
 	while (deps_func(pkg, &d) == EPKG_OK) {
 		dbg(4, "Processing *deps for %s: %s", pkg->uid, d->uid);
-		if (hash_get(universe->items, d->uid) != NULL)
+		universe_itemv_t *known = hash_get_value(universe->items, d->uid);
+		if (known != NULL) {
+			/* Candidate discovery can preload an unprocessed package. */
+			for (size_t i = 0; i < known->len; i++) {
+				unit = known->d[i];
+				if (unit->processed)
+					continue;
+				rc = pkg_jobs_universe_process_item(universe,
+				    unit->pkg, NULL);
+				if (rc != EPKG_OK)
+					return (rc);
+			}
 			continue;
+		}
 
 		rpkgs = NULL;
 		npkg = NULL;
