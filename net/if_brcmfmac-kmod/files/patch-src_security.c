--- src/security.c.orig
+++ src/security.c
@@ -15,6 +15,8 @@
 #include <sys/socket.h>
 #include <sys/sysctl.h>
 
+#include <machine/atomic.h>
+
 #include <net/if.h>
 #include <net/if_var.h>
 #include <net/if_media.h>
@@ -131,6 +133,12 @@
 		IEEE80211_NODE_LOCK(nt);
 	if (error != 0)
 		device_printf(sc->dev, "wsec_key set failed: %d\n", error);
+	else if (!(k->wk_flags & IEEE80211_KEY_GROUP)) {
+		atomic_store_rel_int(&sc->diag_ptk_set_since_join, 1);
+		device_printf(sc->dev,
+		    "link trace: pairwise key installed for join_bssid=%6D\n",
+		    sc->join_bssid, ":");
+	}
 
 	return (error == 0);
 }
@@ -171,6 +179,12 @@
 		IEEE80211_NODE_LOCK(nt);
 	if (error != 0)
 		device_printf(sc->dev, "wsec_key delete failed: %d\n", error);
+	else if (!(k->wk_flags & IEEE80211_KEY_GROUP)) {
+		atomic_store_rel_int(&sc->diag_ptk_set_since_join, 0);
+		device_printf(sc->dev,
+		    "link trace: pairwise key deleted for join_bssid=%6D\n",
+		    sc->join_bssid, ":");
+	}
 
 	return 1;
 }
@@ -275,6 +289,18 @@
 	    "tx_count", CTLFLAG_RD, &sc->tx_count, 0,
 	    "TX packets submitted to flowring");
 	SYSCTL_ADD_UINT(&sc->sysctl_ctx, SYSCTL_CHILDREN(oid), OID_AUTO,
+	    "diag_tx_vap_count", CTLFLAG_RD, &sc->diag_tx_vap_count, 0,
+	    "VAP transmit attempts");
+	SYSCTL_ADD_UINT(&sc->sysctl_ctx, SYSCTL_CHILDREN(oid), OID_AUTO,
+	    "diag_tx_fw_dead", CTLFLAG_RD, &sc->diag_tx_fw_dead, 0,
+	    "TX attempts after firmware was declared dead");
+	SYSCTL_ADD_UINT(&sc->sysctl_ctx, SYSCTL_CHILDREN(oid), OID_AUTO,
+	    "diag_tx_no_flowring", CTLFLAG_RD, &sc->diag_tx_no_flowring, 0,
+	    "TX attempts without a live firmware flow ring");
+	SYSCTL_ADD_UINT(&sc->sysctl_ctx, SYSCTL_CHILDREN(oid), OID_AUTO,
+	    "diag_tx_ring_full", CTLFLAG_RD, &sc->diag_tx_ring_full, 0,
+	    "TX attempts rejected by a full firmware flow ring");
+	SYSCTL_ADD_UINT(&sc->sysctl_ctx, SYSCTL_CHILDREN(oid), OID_AUTO,
 	    "tx_drops", CTLFLAG_RD, &sc->tx_drops, 0,
 	    "TX packets dropped (ring full)");
 	SYSCTL_ADD_UINT(&sc->sysctl_ctx, SYSCTL_CHILDREN(oid), OID_AUTO,
