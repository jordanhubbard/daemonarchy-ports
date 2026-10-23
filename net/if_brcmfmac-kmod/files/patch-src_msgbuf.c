--- src/msgbuf.c.orig
+++ src/msgbuf.c
@@ -17,6 +17,8 @@
 #include <sys/mbuf.h>
 #include <sys/socket.h>
 
+#include <machine/atomic.h>
+
 #include <net/if.h>
 #include <net/if_var.h>
 #include <net/if_media.h>
@@ -257,10 +259,42 @@
 /*
  * Update write index in TCM after writing to ring.
  */
+static uint16_t
+brcmf_ring_idx_read(struct brcmf_softc *sc, uint32_t addr)
+{
+	uint16_t value;
+
+	if (sc->idx_buf == NULL)
+		return (brcmf_tcm_read16(sc, addr));
+	if (addr > sc->idx_buf_sz - sizeof(uint16_t))
+		return (0);
+	bus_dmamap_sync(sc->idx_dma_tag, sc->idx_dma_map,
+	    BUS_DMASYNC_POSTREAD);
+	value = le16toh(*(volatile uint16_t *)((uint8_t *)sc->idx_buf + addr));
+	bus_dmamap_sync(sc->idx_dma_tag, sc->idx_dma_map,
+	    BUS_DMASYNC_PREREAD);
+	return (value);
+}
+
+static void
+brcmf_ring_idx_write(struct brcmf_softc *sc, uint32_t addr, uint16_t value)
+{
+	if (sc->idx_buf == NULL) {
+		brcmf_tcm_write16(sc, addr, value);
+		return;
+	}
+	if (addr > sc->idx_buf_sz - sizeof(uint16_t))
+		return;
+	*(volatile uint16_t *)((uint8_t *)sc->idx_buf + addr) =
+	    htole16(value);
+	bus_dmamap_sync(sc->idx_dma_tag, sc->idx_dma_map,
+	    BUS_DMASYNC_PREWRITE);
+}
+
 static void
 brcmf_ring_write_wptr(struct brcmf_softc *sc, struct brcmf_pcie_ringbuf *ring)
 {
-	brcmf_tcm_write16(sc, ring->w_idx_addr, ring->w_ptr);
+	brcmf_ring_idx_write(sc, ring->w_idx_addr, ring->w_ptr);
 }
 
 /*
@@ -269,7 +303,7 @@
 static uint16_t
 brcmf_ring_read_rptr(struct brcmf_softc *sc, struct brcmf_pcie_ringbuf *ring)
 {
-	ring->r_ptr = brcmf_tcm_read16(sc, ring->r_idx_addr);
+	ring->r_ptr = brcmf_ring_idx_read(sc, ring->r_idx_addr);
 	return (ring->r_ptr);
 }
 
@@ -279,7 +313,7 @@
 static uint16_t
 brcmf_ring_read_wptr(struct brcmf_softc *sc, struct brcmf_pcie_ringbuf *ring)
 {
-	ring->w_ptr = brcmf_tcm_read16(sc, ring->w_idx_addr);
+	ring->w_ptr = brcmf_ring_idx_read(sc, ring->w_idx_addr);
 	return (ring->w_ptr);
 }
 
@@ -289,7 +323,7 @@
 static void
 brcmf_ring_write_rptr(struct brcmf_softc *sc, struct brcmf_pcie_ringbuf *ring)
 {
-	brcmf_tcm_write16(sc, ring->r_idx_addr, ring->r_ptr);
+	brcmf_ring_idx_write(sc, ring->r_idx_addr, ring->r_ptr);
 }
 
 /*
@@ -325,6 +359,8 @@
 void
 brcmf_msgbuf_ring_submit(struct brcmf_softc *sc, struct brcmf_pcie_ringbuf *ring)
 {
+	bus_dmamap_sync(ring->dma_tag, ring->dma_map,
+	    BUS_DMASYNC_PREWRITE);
 	brcmf_ring_write_wptr(sc, ring);
 	brcmf_msgbuf_ring_doorbell(sc);
 }
@@ -389,7 +425,8 @@
 	case BRCMF_E_DISASSOC:
 	case BRCMF_E_DISASSOC_IND:
 		brcmf_link_event(sc, event_code, be32toh(event->msg.status),
-		    be32toh(event->msg.reason), be16toh(event->msg.flags));
+		    be32toh(event->msg.reason), be16toh(event->msg.flags),
+		    event->msg.addr);
 		break;
 	case BRCMF_E_ESCAN_RESULT:
 		if (datalen > 0 && datalen < BRCMF_MSGBUF_MAX_CTL_PKT_SIZE - sizeof(*event))
@@ -954,6 +991,8 @@
 		memcpy(sc->ioctl_reqbuf, buf, len);
 	else
 		buflen = 0;
+	bus_dmamap_sync(sc->ioctl_reqbuf_tag, sc->ioctl_reqbuf_map,
+	    BUS_DMASYNC_PREWRITE);
 
 	memset(req, 0, sizeof(*req));
 	req->msg.msgtype = MSGBUF_TYPE_IOCTLPTR_REQ;
@@ -1037,9 +1076,9 @@
 			    BRCMF_D2H_MSGRING_CONTROL_COMPLETE];
 			if (h2d_ctrl != NULL && d2h_ctrl != NULL) {
 				uint16_t h2d_fw_rptr, d2h_fw_wptr;
-				h2d_fw_rptr = brcmf_tcm_read16(sc,
+				h2d_fw_rptr = brcmf_ring_idx_read(sc,
 				    h2d_ctrl->r_idx_addr);
-				d2h_fw_wptr = brcmf_tcm_read16(sc,
+				d2h_fw_wptr = brcmf_ring_idx_read(sc,
 				    d2h_ctrl->w_idx_addr);
 				device_printf(sc->dev,
 				    "  diag: h2d_ctrl w=%u r=%u "
@@ -1066,9 +1105,19 @@
 		*resp_len = sc->ioctl_resp_len;
 
 	error = 0;
-	if (sc->ioctl_status != 0)
+	if (sc->ioctl_status != 0) {
+		if (sc->chip == 0x4364 && cmd == 262 && buf != NULL &&
+		    len >= 9 && memcmp(buf, "scan_ver", 9) == 0)
+			device_printf(sc->dev,
+			    "BCM4364 scan_ver firmware status=%d\n",
+			    (int16_t)sc->ioctl_status);
+		if (sc->chip == 0x4364 && cmd == 263 && buf != NULL &&
+		    len >= 6 && memcmp(buf, "escan", 6) == 0)
+			device_printf(sc->dev,
+			    "BCM4364 escan firmware status=%d response_len=%u\n",
+			    (int16_t)sc->ioctl_status, sc->ioctl_resp_len);
 		error = EIO;
-	else if (buf != NULL && sc->ioctl_resp_len > 0) {
+	} else if (buf != NULL && sc->ioctl_resp_len > 0) {
 		if (sc->ioctl_resp_len > len)
 			sc->ioctl_resp_len = len;
 		memcpy(buf, sc->ioctl_respbuf, sc->ioctl_resp_len);
@@ -1246,10 +1295,20 @@
 	/* Calculate index addresses in TCM (flowrings start after common H2D rings).
 	 * TCM index slots are 4 bytes each, though only lower 16 bits used.
 	 */
-	ring->w_idx_addr = sc->h2d_w_idx_addr +
-	    (BRCMF_NROF_H2D_COMMON_MSGRINGS + flowid) * sizeof(uint32_t);
-	ring->r_idx_addr = sc->h2d_r_idx_addr +
-	    (BRCMF_NROF_H2D_COMMON_MSGRINGS + flowid) * sizeof(uint32_t);
+	if (sc->idx_buf != NULL) {
+		ring->w_idx_addr =
+		    (BRCMF_NROF_H2D_COMMON_MSGRINGS + flowid) *
+		    sc->shared.dma_idx_sz;
+		ring->r_idx_addr =
+		    (sc->max_submissionrings +
+		    BRCMF_NROF_H2D_COMMON_MSGRINGS + flowid) *
+		    sc->shared.dma_idx_sz;
+	} else {
+		ring->w_idx_addr = sc->h2d_w_idx_addr +
+		    (BRCMF_NROF_H2D_COMMON_MSGRINGS + flowid) * sizeof(uint32_t);
+		ring->r_idx_addr = sc->h2d_r_idx_addr +
+		    (BRCMF_NROF_H2D_COMMON_MSGRINGS + flowid) * sizeof(uint32_t);
+	}
 
 	/* Write ring descriptor to TCM (flowrings start after common rings) */
 	desc_addr = sc->ringmem_addr +
@@ -1330,10 +1389,16 @@
 	int error, nsegs;
 	uint32_t pktid;
 
-	if (sc->fw_dead || sc->flowring == NULL) {
+	if (sc->fw_dead) {
+		atomic_add_int(&sc->diag_tx_fw_dead, 1);
 		m_freem(m);
 		return (ENXIO);
 	}
+	if (sc->flowring == NULL) {
+		atomic_add_int(&sc->diag_tx_no_flowring, 1);
+		m_freem(m);
+		return (ENXIO);
+	}
 
 	ring = sc->flowring;
 
@@ -1393,6 +1458,7 @@
 	/* Reserve space in flow ring */
 	tx = brcmf_msgbuf_ring_reserve(sc, ring);
 	if (tx == NULL) {
+		atomic_add_int(&sc->diag_tx_ring_full, 1);
 		bus_dmamap_unload(txb->dma_tag, txb->dma_map);
 		txb->m = NULL;
 		m_freem(m);
@@ -1437,6 +1503,10 @@
 	if (error != 0) {
 		device_printf(sc->dev, "failed to allocate IOCTL buffer\n");
 		return (error);
+	}
+	if (sc->chip == 0x4364) {
+		device_printf(sc->dev, "BCM4364 IOCTL request buffer ready\n");
+		pause_sbt("brcmlog", mstosbt(50), 0, 0);
 	}
 
 
@@ -1447,18 +1517,28 @@
 		    "failed to post IOCTL response buffers\n");
 		return (error);
 	}
+	if (sc->chip == 0x4364) {
+		device_printf(sc->dev, "BCM4364 IOCTL response buffers posted\n");
+		pause_sbt("brcmlog", mstosbt(50), 0, 0);
+	}
 
 	error = brcmf_msgbuf_init_event(sc);
 	if (error != 0) {
 		device_printf(sc->dev, "failed to post event buffers\n");
 		return (error);
 	}
+	if (sc->chip == 0x4364) {
+		device_printf(sc->dev, "BCM4364 event buffers posted\n");
+		pause_sbt("brcmlog", mstosbt(50), 0, 0);
+	}
 
 	error = brcmf_msgbuf_init_rxbuf(sc);
 	if (error != 0) {
 		device_printf(sc->dev, "failed to post RX buffers\n");
 		return (error);
 	}
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 RX buffers posted\n");
 
 	return (0);
 }
