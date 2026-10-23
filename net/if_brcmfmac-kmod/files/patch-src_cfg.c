--- src/cfg.c.orig
+++ src/cfg.c
@@ -973,6 +973,15 @@
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
