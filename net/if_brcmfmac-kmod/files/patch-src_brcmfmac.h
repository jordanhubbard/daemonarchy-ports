--- src/brcmfmac.h.orig
+++ src/brcmfmac.h
@@ -67,6 +67,7 @@
 /* PCI IDs */
 #define PCI_VENDOR_BROADCOM 0x14e4
 #define PCI_DEVICE_BCM4350  0x43a3
+#define PCI_DEVICE_BCM4364  0x4464
 
 /* Device ID table entry */
 struct brcmf_dev_id {
@@ -449,6 +450,8 @@
     void *data, uint32_t len);
 int brcmf_fil_iovar_data_set(struct brcmf_softc *sc, const char *name,
     const void *data, uint32_t len);
+void brcmf_load_blob(struct brcmf_softc *sc, const char *fwname,
+    const char *loadvar, const char *statusvar);
 int brcmf_fil_bsscfg_data_set(struct brcmf_softc *sc, const char *name,
     int bsscfg_idx, const void *data, uint32_t len);
 int brcmf_fil_iovar_int_set(struct brcmf_softc *sc, const char *name,
