--- src/main.c.orig
+++ src/main.c
@@ -37,6 +37,7 @@
 
 static const struct brcmf_dev_id brcmf_devid_table[] = {
 	{ PCI_VENDOR_BROADCOM, PCI_DEVICE_BCM4350, "Broadcom BCM4350 WiFi" },
+	{ PCI_VENDOR_BROADCOM, PCI_DEVICE_BCM4364, "Broadcom BCM4364 WiFi (startup probe)" },
 	{ 0, 0, NULL }
 };
 
@@ -117,8 +118,9 @@
 #define DLOAD_HANDLER_VER    1
 #define DLOAD_FLAG_VER_SHIFT 12
 
-static void
-brcmf_sdio_load_clm(struct brcmf_softc *sc)
+void
+brcmf_load_blob(struct brcmf_softc *sc, const char *fwname,
+    const char *loadvar, const char *statusvar)
 {
 	const struct firmware *fw;
 	struct brcmf_dload_data_le *chunk;
@@ -127,14 +129,15 @@
 	uint32_t status;
 	int error;
 
-	fw = firmware_get(BRCMF_CLM_FW_NAME);
+	fw = firmware_get(fwname);
 	if (fw == NULL) {
 		device_printf(sc->dev,
-		    "no CLM blob available, channels may be limited\n");
+		    "no %s blob available\n", loadvar);
 		return;
 	}
 
-	device_printf(sc->dev, "loading CLM blob (%zu bytes)\n", fw->datasize);
+	device_printf(sc->dev, "loading %s blob (%zu bytes)\n", loadvar,
+	    fw->datasize);
 
 	chunk = malloc(sizeof(*chunk) + BRCMF_CLM_MAX_CHUNK, M_BRCMFMAC,
 	    M_WAITOK | M_ZERO);
@@ -165,12 +168,11 @@
 		 * iovar buffer manually and call ioctl directly.
 		 */
 		{
-			static const char clmload[] = "clmload";
-			uint32_t namelen = sizeof(clmload);
+			uint32_t namelen = strlen(loadvar) + 1;
 			uint32_t paylen = sizeof(*chunk) + chunk_len;
 			uint32_t total = namelen + paylen;
 			uint8_t *iobuf = malloc(total, M_BRCMFMAC, M_WAITOK);
-			memcpy(iobuf, clmload, namelen);
+			memcpy(iobuf, loadvar, namelen);
 			memcpy(iobuf + namelen, chunk, paylen);
 			error = sc->bus_ops->ioctl(sc, 263 /* C_SET_VAR */, 1,
 			    iobuf, total, NULL);
@@ -178,8 +180,8 @@
 		}
 		if (error != 0) {
 			device_printf(sc->dev,
-			    "CLM download failed at offset %u: %d\n",
-			    cumulative, error);
+			    "%s download failed at offset %u: %d\n",
+			    loadvar, cumulative, error);
 			break;
 		}
 
@@ -192,11 +194,12 @@
 	firmware_put(fw, FIRMWARE_UNLOAD);
 
 	if (error == 0) {
-		error = brcmf_fil_iovar_int_get(sc, "clmload_status", &status);
+		error = brcmf_fil_iovar_int_get(sc, statusvar, &status);
 		if (error == 0 && status != 0) {
-			device_printf(sc->dev, "CLM load status: %u\n", status);
+			device_printf(sc->dev, "%s status: %u\n", loadvar,
+			    status);
 		} else if (error == 0) {
-			device_printf(sc->dev, "CLM blob loaded\n");
+			device_printf(sc->dev, "%s blob loaded\n", loadvar);
 		}
 	}
 }
@@ -255,7 +258,8 @@
 	brcmf_fil_cmd_data_set(sc, 89 /* C_SET_GLOM */,
 	    &(uint32_t) { htole32(0) }, sizeof(uint32_t));
 
-	brcmf_sdio_load_clm(sc);
+	brcmf_load_blob(sc, BRCMF_CLM_FW_NAME, "clmload",
+	    "clmload_status");
 	return (0);
 }
 
