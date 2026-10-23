--- src/brcmfmac.h.orig
+++ src/brcmfmac.h
@@ -67,6 +67,7 @@
 /* PCI IDs */
 #define PCI_VENDOR_BROADCOM 0x14e4
 #define PCI_DEVICE_BCM4350  0x43a3
+#define PCI_DEVICE_BCM4364  0x4464
 
 /* Device ID table entry */
 struct brcmf_dev_id {
@@ -301,6 +302,10 @@
 	uint32_t isr_task_count;
 	uint32_t tx_count;
 	uint32_t tx_drops;
+	u_int diag_tx_vap_count;
+	u_int diag_tx_fw_dead;
+	u_int diag_tx_no_flowring;
+	u_int diag_tx_ring_full;
 	uint32_t tx_complete_count;
 	uint32_t rx_complete_count;
 	uint32_t rx_deliver_fail;  /* m_get2 allocation failures */
@@ -331,6 +336,7 @@
 
 	/* Link state */
 	int link_up;
+	volatile u_int diag_ptk_set_since_join;
 	struct task link_task;
 	struct task restart_task;
 	uint8_t join_bssid[6];
@@ -449,6 +455,8 @@
     void *data, uint32_t len);
 int brcmf_fil_iovar_data_set(struct brcmf_softc *sc, const char *name,
     const void *data, uint32_t len);
+void brcmf_load_blob(struct brcmf_softc *sc, const char *fwname,
+    const char *loadvar, const char *statusvar);
 int brcmf_fil_bsscfg_data_set(struct brcmf_softc *sc, const char *name,
     int bsscfg_idx, const void *data, uint32_t len);
 int brcmf_fil_iovar_int_set(struct brcmf_softc *sc, const char *name,
@@ -463,7 +471,8 @@
 void brcmf_cfg_detach(struct brcmf_softc *sc);
 void brcmf_escan_result(struct brcmf_softc *sc, void *data, uint32_t datalen);
 void brcmf_link_event(struct brcmf_softc *sc, uint32_t event_code,
-    uint32_t status, uint32_t reason, uint16_t flags);
+    uint32_t status, uint32_t reason, uint16_t flags,
+    const uint8_t event_bssid[6]);
 
 /* core.c - chip parsing and EROM scan helpers */
 struct brcmf_chipinfo brcmf_parse_chipid(uint32_t regdata);
