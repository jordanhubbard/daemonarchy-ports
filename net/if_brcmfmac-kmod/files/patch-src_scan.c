--- src/scan.c.orig
+++ src/scan.c
@@ -27,6 +27,49 @@
 static int brcmf_ie_has_rsn_wpa(const uint8_t *ie, uint16_t ie_len);
 static int brcmf_find_security_ie(const uint8_t *raw, uint32_t start,
     uint32_t bi_len, const uint8_t **ie, uint32_t *ie_len);
+
+/* Never hand a truncated cipher/AKM list to net80211 or ifconfig. */
+static int
+brcmf_valid_security_ie(const uint8_t *ie)
+{
+	const uint8_t *p = ie + 2, *end = p + ie[1];
+	uint16_t count;
+	int rsn = ie[0] == IEEE80211_ELEMID_RSN;
+
+	if (!rsn) {
+		if (ie[0] != IEEE80211_ELEMID_VENDOR || end - p < 4 ||
+		    p[0] != 0x00 || p[1] != 0x50 || p[2] != 0xf2 || p[3] != 0x01)
+			return (0);
+		p += 4;
+	}
+	if (end - p < 8 || p[0] != 1 || p[1] != 0)
+		return (0);
+	p += 6; /* version and group cipher */
+	count = p[0] | ((uint16_t)p[1] << 8);
+	p += 2;
+	if (count == 0 || count > (end - p) / 4)
+		return (0);
+	p += count * 4;
+	if (end - p < 2)
+		return (0);
+	count = p[0] | ((uint16_t)p[1] << 8);
+	p += 2;
+	if (count == 0 || count > (end - p) / 4)
+		return (0);
+	p += count * 4;
+	if (end - p >= 2)
+		p += 2; /* capabilities */
+	if (rsn && end - p >= 2) {
+		count = p[0] | ((uint16_t)p[1] << 8);
+		p += 2;
+		if (count > (end - p) / 16)
+			return (0);
+		p += count * 16;
+	}
+	if (rsn && end - p == 4)
+		p += 4; /* group management cipher */
+	return (p == end);
+}
 
 static int
 brcmf_chanspec_to_channel_d11ac(uint16_t chanspec)
@@ -166,6 +209,18 @@
 	error = brcmf_fil_iovar_data_set(sc, "escan", params, params_size);
 	if (error != 0) {
 		device_printf(sc->dev, "escan failed: %d\n", error);
+		if (sc->chip == 0x4364) {
+			char reason[128];
+			int reason_error;
+
+			memset(reason, 0, sizeof(reason));
+			reason_error = brcmf_fil_iovar_data_get(sc,
+			    "bcmerrorstr", reason, sizeof(reason) - 1);
+			device_printf(sc->dev,
+			    "BCM4364 bcmerrorstr error=%d value=%.100s\n",
+			    reason_error, reason);
+			brcmf_pcie_console_read(sc);
+		}
 		sc->scan_active = 0;
 	}
 
@@ -468,10 +523,12 @@
 
 		if (p + 2 + len > end)
 			break;
-		if (id == IEEE80211_ELEMID_RSN)
+		if (id == IEEE80211_ELEMID_RSN &&
+		    brcmf_valid_security_ie(p))
 			return (1);
 		if (id == IEEE80211_ELEMID_VENDOR && len >= 4 && p[2] == 0x00 &&
-		    p[3] == 0x50 && p[4] == 0xf2 && p[5] == 0x01)
+		    p[3] == 0x50 && p[4] == 0xf2 && p[5] == 0x01 &&
+		    brcmf_valid_security_ie(p))
 			return (1);
 		p += 2 + len;
 	}
@@ -491,14 +548,16 @@
 
 		if (j + 2 + len > bi_len)
 			continue;
-		if (id == IEEE80211_ELEMID_RSN) {
+		if (id == IEEE80211_ELEMID_RSN &&
+		    brcmf_valid_security_ie(raw + j)) {
 			*ie = raw + j;
 			*ie_len = 2 + len;
 			return (1);
 		}
 		if (id == IEEE80211_ELEMID_VENDOR && len >= 4 &&
 		    raw[j + 2] == 0x00 && raw[j + 3] == 0x50 &&
-		    raw[j + 4] == 0xf2 && raw[j + 5] == 0x01) {
+		    raw[j + 4] == 0xf2 && raw[j + 5] == 0x01 &&
+		    brcmf_valid_security_ie(raw + j)) {
 			*ie = raw + j;
 			*ie_len = 2 + len;
 			return (1);
@@ -566,7 +625,8 @@
 			sp->country = p;
 			break;
 		case IEEE80211_ELEMID_RSN:
-			sp->rsn = p;
+			if (brcmf_valid_security_ie(p))
+				sp->rsn = p;
 			break;
 		case IEEE80211_ELEMID_HTCAP:
 			sp->htcap = p;
@@ -576,7 +636,8 @@
 			break;
 		case IEEE80211_ELEMID_VENDOR:
 			if (len >= 4 && p[2] == 0x00 && p[3] == 0x50 &&
-			    p[4] == 0xf2 && p[5] == 0x01)
+			    p[4] == 0xf2 && p[5] == 0x01 &&
+			    brcmf_valid_security_ie(p))
 				sp->wpa = p;
 			if (len >= 4 && p[2] == 0x00 && p[3] == 0x50 &&
 			    p[4] == 0xf2 && p[5] == 0x02)
