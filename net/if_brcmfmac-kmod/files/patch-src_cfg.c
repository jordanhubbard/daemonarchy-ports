--- src/cfg.c.orig
+++ src/cfg.c
@@ -21,6 +21,8 @@
 #include <sys/sysctl.h>
 #include <sys/taskqueue.h>
 
+#include <machine/atomic.h>
+
 #include <net/ethernet.h>
 #include <net/if.h>
 #include <net/if_media.h>
@@ -154,8 +156,19 @@
  */
 void
 brcmf_link_event(struct brcmf_softc *sc, uint32_t event_code, uint32_t status,
-    uint32_t reason, uint16_t flags)
+    uint32_t reason, uint16_t flags, const uint8_t event_bssid[6])
 {
+	struct ieee80211vap *trace_vap;
+
+	trace_vap = TAILQ_FIRST(&sc->ic.ic_vaps);
+	device_printf(sc->dev,
+	    "link trace: event=%u status=%u reason=%u flags=0x%x "
+	    "event_bssid=%6D join_bssid=%6D ptk_set_since_join=%u "
+	    "link_up=%d vap_state=%d\n",
+	    event_code, status, reason, flags, event_bssid, ":",
+	    sc->join_bssid, ":",
+	    atomic_load_acq_int(&sc->diag_ptk_set_since_join), sc->link_up,
+	    trace_vap != NULL ? trace_vap->iv_state : -1);
 	switch (event_code) {
 	case BRCMF_E_SET_SSID:
 		if (status == BRCMF_E_STATUS_SUCCESS) {
@@ -203,8 +216,12 @@
 
 		/* Ignore spurious link-down during WPA handshake.
 		 * The firmware sends E_LINK(down) before PTK is installed,
-		 * which would abort the handshake prematurely. */
-		if (!link && vap != NULL && vap->iv_state == IEEE80211_S_RUN) {
+		 * which would abort the handshake prematurely. Once the PTK
+		 * is in, a link-down is real (beacon loss, AP gone): the
+		 * firmware has dropped the association and tosses every
+		 * data frame, so net80211 and wpa_supplicant must be told. */
+		if (!link && vap != NULL && vap->iv_state == IEEE80211_S_RUN &&
+		    atomic_load_acq_int(&sc->diag_ptk_set_since_join) == 0) {
 			device_printf(sc->dev, "ignoring E_LINK down in RUN\n");
 			break;
 		}
@@ -265,6 +282,7 @@
 		brcmf_fil_iovar_int_set(sc, "sup_wpa", 0);
 
 	memcpy(sc->join_bssid, sr->bssid, 6);
+	atomic_store_rel_int(&sc->diag_ptk_set_since_join, 0);
 
 	brcmf_abort_escan(sc);
 
@@ -298,6 +316,7 @@
 	uint16_t chanspec;
 
 	memcpy(sc->join_bssid, bssid, 6);
+	atomic_store_rel_int(&sc->diag_ptk_set_since_join, 0);
 
 	brcmf_abort_escan(sc);
 
@@ -880,15 +899,7 @@
 	struct ieee80211vap *vap = if_getsoftc(ifp);
 	struct ieee80211com *ic = vap->iv_ic;
 	struct brcmf_softc *sc = ic->ic_softc;
-	struct ether_header *eh;
-	uint16_t etype = 0;
-
-	if (m->m_len >= sizeof(*eh)) {
-		eh = mtod(m, struct ether_header *);
-		etype = ntohs(eh->ether_type);
-	}
-	device_printf(sc->dev, "vap_transmit: state=%d etype=0x%04x len=%u\n",
-	    vap->iv_state, etype, m->m_pkthdr.len);
+	atomic_add_int(&sc->diag_tx_vap_count, 1);
 
 	if (vap->iv_state != IEEE80211_S_RUN) {
 		/*
@@ -973,6 +984,15 @@
 	error = brcmf_get_macaddr(sc);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364) {
+		uint32_t scan_ver = 0;
+		int scan_ver_error;
+
+		scan_ver_error = brcmf_fil_iovar_int_get(sc, "scan_ver",
+		    &scan_ver);
+		device_printf(sc->dev, "BCM4364 scan_ver error=%d value=%u\n",
+		    scan_ver_error, scan_ver);
+	}
 
 	/* Determine chanspec encoding: D11N (io_type=1) or D11AC (io_type=2) */
 	{
