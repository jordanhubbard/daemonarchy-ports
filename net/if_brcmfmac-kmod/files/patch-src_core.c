--- src/core.c.orig
+++ src/core.c
@@ -271,7 +271,7 @@
 bool
 brcmf_chip_supported(uint32_t chip)
 {
-	return (chip == BRCM_CC_4350_CHIP_ID);
+	return (chip == BRCM_CC_4350_CHIP_ID || chip == 0x4364);
 }
 
 const char *
