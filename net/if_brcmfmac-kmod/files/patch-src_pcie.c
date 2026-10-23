--- src/pcie.c.orig
+++ src/pcie.c
@@ -42,6 +42,12 @@
 /* BCM4350 RAM info */
 #define BCM4350_RAM_BASE 0x180000
 #define BCM4350_RAM_SIZE 0xC0000 /* 768KB */
+
+/* MacBookPro16,1 BCM4364/4, measured with ARM CR4 TCM bank registers. */
+#define BCM4364_RAM_BASE 0x160000
+#define BCM4364_RAM_SIZE 0x140000
+#define BCM4364_FW_NAME  "brcmfmac4364b3-pcie.bin"
+#define BCM4364_NVRAM_NAME "brcmfmac4364b3-pcie.txt"
 
 /* BCM4350 note: spec defines two register layouts (core rev < 64
  * and >= 64). BCM4350 is rev < 64, so only legacy offsets are used. */
@@ -428,7 +434,7 @@
 	uint64_t dma64;
 	int error;
 
-	if ((sc->shared.flags & BRCMF_PCIE_SHARED_DMA_INDEX) == 0)
+	if (sc->shared.dma_idx_sz == 0)
 		return (0);
 
 	sz = sc->shared.dma_idx_sz *
@@ -455,6 +461,8 @@
 	dma64 += sc->max_completionrings * sc->shared.dma_idx_sz;
 
 	brcmf_tcm_write64(sc, addr + BRCMF_RING_D2H_R_HOSTADDR_OFFSET, dma64);
+	bus_dmamap_sync(sc->idx_dma_tag, sc->idx_dma_map,
+	    BUS_DMASYNC_PREWRITE | BUS_DMASYNC_PREREAD);
 
 	return (0);
 }
@@ -551,18 +559,42 @@
 			    "failed to allocate common ring %d\n", i);
 			return (ENOMEM);
 		}
+		if (sc->chip == 0x4364) {
+			device_printf(sc->dev, "BCM4364 ring %d allocated\n", i);
+			pause_sbt("brcmlog", mstosbt(50), 0, 0);
+		}
 
 		if (i < BRCMF_NROF_H2D_COMMON_MSGRINGS) {
-			sc->commonrings[i]->w_idx_addr = sc->h2d_w_idx_addr +
-			    i * sizeof(uint32_t);
-			sc->commonrings[i]->r_idx_addr = sc->h2d_r_idx_addr +
-			    i * sizeof(uint32_t);
+			if (sc->idx_buf != NULL) {
+				sc->commonrings[i]->w_idx_addr =
+				    i * sc->shared.dma_idx_sz;
+				sc->commonrings[i]->r_idx_addr =
+				    (sc->max_submissionrings + i) *
+				    sc->shared.dma_idx_sz;
+			} else {
+				sc->commonrings[i]->w_idx_addr =
+				    sc->h2d_w_idx_addr + i * sizeof(uint32_t);
+				sc->commonrings[i]->r_idx_addr =
+				    sc->h2d_r_idx_addr + i * sizeof(uint32_t);
+			}
 		} else {
 			int d2h_idx = i - BRCMF_NROF_H2D_COMMON_MSGRINGS;
-			sc->commonrings[i]->w_idx_addr = sc->d2h_w_idx_addr +
-			    d2h_idx * sizeof(uint32_t);
-			sc->commonrings[i]->r_idx_addr = sc->d2h_r_idx_addr +
-			    d2h_idx * sizeof(uint32_t);
+			if (sc->idx_buf != NULL) {
+				sc->commonrings[i]->w_idx_addr =
+				    (2 * sc->max_submissionrings + d2h_idx) *
+				    sc->shared.dma_idx_sz;
+				sc->commonrings[i]->r_idx_addr =
+				    (2 * sc->max_submissionrings +
+				    sc->max_completionrings + d2h_idx) *
+				    sc->shared.dma_idx_sz;
+			} else {
+				sc->commonrings[i]->w_idx_addr =
+				    sc->d2h_w_idx_addr +
+				    d2h_idx * sizeof(uint32_t);
+				sc->commonrings[i]->r_idx_addr =
+				    sc->d2h_r_idx_addr +
+				    d2h_idx * sizeof(uint32_t);
+			}
 		}
 
 	}
@@ -577,23 +609,36 @@
 brcmf_pcie_setup_rings(struct brcmf_softc *sc)
 {
 	int error;
+	if (sc->chip == 0x4364) {
+		device_printf(sc->dev, "BCM4364 ring setup begin\n");
+		pause_sbt("brcmlog", mstosbt(50), 0, 0);
+	}
 
 	error = brcmf_pcie_init_ringinfo(sc);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 ring info read\n");
 
 	error = brcmf_pcie_alloc_idx_buf(sc);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 DMA indices %s\n",
+		    sc->idx_buf == NULL ? "disabled" : "allocated");
 
 	/* Linux driver order: common rings first, then scratch buffers */
 	error = brcmf_pcie_alloc_common_rings(sc);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 common rings ready\n");
 
 	error = brcmf_pcie_alloc_scratch_buffers(sc);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 scratch buffers ready\n");
 
 	return (0);
 }
@@ -673,16 +718,16 @@
 	struct brcmf_softc *sc = arg;
 	uint32_t status;
 
-	brcmf_pcie_select_core(sc, &sc->pciecore);
 	status = brcmf_reg_read(sc, BRCMF_PCIE_PCIE2REG_MAILBOXINT);
 	if (status == 0 || status == 0xffffffff)
 		return (FILTER_STRAY);
 
-	/* Ack and disable further interrupts until task runs */
-	brcmf_reg_write(sc, BRCMF_PCIE_PCIE2REG_MAILBOXINT, status);
-	brcmf_pcie_intr_disable(sc);
+	/* Preserve status until the deferred task can acknowledge it. */
+	brcmf_reg_write(sc, BRCMF_PCIE_PCIE2REG_MAILBOXMASK, 0);
 
 	sc->isr_filter_count++;
+	if (sc->chip == 0x4364)
+		return (FILTER_SCHEDULE_THREAD);
 	taskqueue_enqueue(sc->isr_tq, &sc->isr_task);
 
 	return (FILTER_HANDLED);
@@ -694,20 +739,34 @@
 	struct brcmf_softc *sc = arg;
 	uint32_t status;
 
-	/* Re-read in case new events arrived between filter and task */
-	brcmf_pcie_select_core(sc, &sc->pciecore);
+	/* Read and acknowledge the status saved by the disabled mask. */
 	status = brcmf_reg_read(sc, BRCMF_PCIE_PCIE2REG_MAILBOXINT);
 	if (status != 0 && status != 0xffffffff)
 		brcmf_reg_write(sc, BRCMF_PCIE_PCIE2REG_MAILBOXINT, status);
 
 	sc->isr_task_count++;
+	if (sc->chip == 0x4364) {
+		if (status & BRCMF_PCIE_MB_INT_FN0)
+			brcmf_pcie_handle_mb_data(sc);
+		if (status & BRCMF_PCIE_MB_INT_D2H_DB)
+			brcmf_msgbuf_process_d2h(sc);
+		brcmf_pcie_intr_enable(sc);
+		return;
+	}
 
 	if (status & BRCMF_PCIE_MB_INT_FN0)
 		brcmf_pcie_handle_mb_data(sc);
 
-	brcmf_msgbuf_process_d2h(sc);
+	if (status & BRCMF_PCIE_MB_INT_D2H_DB)
+		brcmf_msgbuf_process_d2h(sc);
 
 	brcmf_pcie_intr_enable(sc);
+}
+
+static void
+brcmf_pcie_isr_thread(void *arg)
+{
+	brcmf_pcie_isr_task(arg, 0);
 }
 
 /*
@@ -733,14 +792,17 @@
 		return (ENXIO);
 	}
 
-	TASK_INIT(&sc->isr_task, 0, brcmf_pcie_isr_task, sc);
-	sc->isr_tq = taskqueue_create("brcmfmac_isr", M_WAITOK,
-	    taskqueue_thread_enqueue, &sc->isr_tq);
-	taskqueue_start_threads(&sc->isr_tq, 1, PI_NET, "%s isr",
-	    device_get_nameunit(sc->dev));
+	if (sc->chip != 0x4364) {
+		TASK_INIT(&sc->isr_task, 0, brcmf_pcie_isr_task, sc);
+		sc->isr_tq = taskqueue_create("brcmfmac_isr", M_WAITOK,
+		    taskqueue_thread_enqueue, &sc->isr_tq);
+		taskqueue_start_threads(&sc->isr_tq, 1, PI_NET, "%s isr",
+		    device_get_nameunit(sc->dev));
+	}
 
 	error = bus_setup_intr(sc->dev, sc->irq_res,
-	    INTR_TYPE_NET | INTR_MPSAFE, brcmf_pcie_isr_filter, NULL, sc,
+	    INTR_TYPE_NET | INTR_MPSAFE, brcmf_pcie_isr_filter,
+	    sc->chip == 0x4364 ? brcmf_pcie_isr_thread : NULL, sc,
 	    &sc->irq_handle);
 	if (error != 0) {
 		device_printf(sc->dev, "failed to setup interrupt: %d\n",
@@ -1037,18 +1099,43 @@
 
 	resetintr = *(const uint32_t *)fw->data;
 	brcmf_tcm_copy(sc, sc->ram_base, fw->data, fw->datasize);
+	device_printf(sc->dev, "FW readback start=0x%08x expected=0x%08x tail=0x%08x\n",
+	    brcmf_tcm_read32(sc, sc->ram_base), resetintr,
+	    brcmf_tcm_read32(sc, sc->ram_base + fw->datasize - 4));
 
 	brcmf_ram_write32(sc, sc->ram_size - 4, 0);
 
 	if (sc->nvram != NULL && sc->nvram_len > 0) {
 		uint32_t nvram_addr = sc->ram_base + sc->ram_size -
 		    sc->nvram_len;
+		uint8_t random_seed[0x100];
 		brcmf_tcm_copy(sc, nvram_addr, sc->nvram, sc->nvram_len);
+		if (sc->chip == 0x4364) {
+			arc4random_buf(random_seed, sizeof(random_seed));
+			brcmf_tcm_copy(sc, nvram_addr - 8 - sizeof(random_seed),
+			    random_seed, sizeof(random_seed));
+			brcmf_tcm_write32(sc, nvram_addr - 8,
+			    sizeof(random_seed));
+			brcmf_tcm_write32(sc, nvram_addr - 4, 0xfeedc0de);
+			device_printf(sc->dev,
+			    "Apple seed footer at 0x%x: len=%u magic=0x%x\n",
+			    nvram_addr - 8,
+			    brcmf_tcm_read32(sc, nvram_addr - 8),
+			    brcmf_tcm_read32(sc, nvram_addr - 4));
+		}
 	}
 
 	sharedaddr_written = brcmf_ram_read32(sc, sc->ram_size - 4);
+	device_printf(sc->dev, "NVRAM len=%u token=0x%08x ARM ioctl=0x%08x reset=0x%08x\n",
+	    sc->nvram_len, sharedaddr_written,
+	    brcmf_bp_read32(sc, sc->armcore.wrapbase + 0x408),
+	    brcmf_bp_read32(sc, sc->armcore.wrapbase + 0x800));
 
 	brcmf_chip_exit_download(sc, resetintr);
+	device_printf(sc->dev, "ARM after release ioctl=0x%08x reset=0x%08x vector=0x%08x\n",
+	    brcmf_bp_read32(sc, sc->armcore.wrapbase + 0x408),
+	    brcmf_bp_read32(sc, sc->armcore.wrapbase + 0x800),
+	    brcmf_tcm_read32(sc, 0));
 
 	sharedaddr = sharedaddr_written;
 	for (i = 0; i < BRCMF_FW_READY_TIMEOUT_MS / BRCMF_FW_READY_POLL_MS;
@@ -1075,20 +1162,69 @@
 	error = brcmf_pcie_init_shared(sc, sharedaddr);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364) {
+		device_printf(sc->dev,
+		    "BCM4364 shared version=%u flags=0x%x ringinfo=0x%x "
+		    "console=0x%x max_rxbufpost=%u\n",
+		    sc->shared.version, sc->shared.flags,
+		    sc->shared.ring_info_addr, sc->shared.console_addr,
+		    sc->shared.max_rxbufpost);
+		if (sc->shared.ring_info_addr < sc->ram_base ||
+		    sc->shared.ring_info_addr - sc->ram_base > sc->ram_size - 80) {
+			device_printf(sc->dev, "invalid BCM4364 ring info address\n");
+			return (EIO);
+		}
+		error = brcmf_pcie_init_ringinfo(sc);
+		if (error != 0)
+			return (error);
+		device_printf(sc->dev,
+		    "BCM4364 ring limits flow=%u submit=%u complete=%u "
+		    "ringmem=0x%x\n",
+		    sc->max_flowrings, sc->max_submissionrings,
+		    sc->max_completionrings, sc->ringmem_addr);
+		device_printf(sc->dev,
+		    "BCM4364 ring pointers h2d_w=0x%x h2d_r=0x%x "
+		    "d2h_w=0x%x d2h_r=0x%x DMA-index=%u size=%u\n",
+		    sc->h2d_w_idx_addr, sc->h2d_r_idx_addr,
+		    sc->d2h_w_idx_addr, sc->d2h_r_idx_addr,
+		    !!(sc->shared.flags & BRCMF_PCIE_SHARED_DMA_INDEX),
+		    sc->shared.dma_idx_sz);
+		if (sc->max_flowrings > 512 ||
+		    sc->max_submissionrings < 2 ||
+		    sc->max_submissionrings > 514 ||
+		    sc->max_completionrings < 3 ||
+		    sc->max_completionrings > 512 ||
+		    sc->ringmem_addr < sc->ram_base ||
+		    sc->ringmem_addr - sc->ram_base > sc->ram_size - 5 * 16) {
+			device_printf(sc->dev, "invalid BCM4364 ring limits\n");
+			return (EIO);
+		}
+		sc->shared.dma_idx_sz = 0;
+		device_printf(sc->dev,
+		    "BCM4364 diagnostic TCM-index fallback selected\n");
+	}
 
 	error = brcmf_pcie_setup_rings(sc);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 DMA rings ready\n");
 
 	brcmf_pcie_hostready(sc);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 host-ready doorbell sent\n");
 
 	error = brcmf_pcie_setup_irq(sc);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 MSI handler ready\n");
 
 	error = brcmf_msgbuf_init(sc);
 	if (error != 0)
 		return (error);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 receive buffers posted\n");
 
 	brcmf_pcie_intr_enable(sc);
 
@@ -1100,6 +1236,9 @@
 		memset(ver, 0, sizeof(ver));
 		error = brcmf_fil_iovar_data_get(sc, "ver", ver,
 		    sizeof(ver) - 1);
+		if (sc->chip == 0x4364)
+			device_printf(sc->dev,
+			    "BCM4364 firmware-version request result=%d\n", error);
 		if (error == 0) {
 			char *nl = strchr(ver, '\n');
 			if (nl)
@@ -1110,8 +1249,11 @@
 
 	/* BCM4350 note: spec default is mpc=1, but we use 0 to avoid
 	 * ~7ms wake latency on first TX after idle. */
-	brcmf_fil_iovar_int_set(sc, "mpc", 0);
-
+	error = brcmf_fil_iovar_int_set(sc, "mpc", 0);
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev,
+		    "BCM4364 transport setup result=%d filter=%u task=%u\n",
+		    error, sc->isr_filter_count, sc->isr_task_count);
 	return (0);
 }
 
@@ -1177,13 +1319,25 @@
 		goto fail;
 	}
 
-	sc->ram_base = BCM4350_RAM_BASE;
-	sc->ram_size = BCM4350_RAM_SIZE;
+	if (sc->chip == 0x4364) {
+		if (sc->chiprev != 4) {
+			device_printf(dev, "unqualified BCM4364 revision %u\n",
+			    sc->chiprev);
+			error = ENODEV;
+			goto fail;
+		}
+		sc->ram_base = BCM4364_RAM_BASE;
+		sc->ram_size = BCM4364_RAM_SIZE;
+	} else {
+		sc->ram_base = BCM4350_RAM_BASE;
+		sc->ram_size = BCM4350_RAM_SIZE;
+	}
 
 	BRCMF_DBG(sc, "ram_base=0x%x ram_size=0x%x (%uKB)\n", sc->ram_base,
 	    sc->ram_size, sc->ram_size / 1024);
 
-	fwname = (sc->chiprev <= 7) ? BRCMF_FW_NAME_C2 : BRCMF_FW_NAME;
+	fwname = sc->chip == 0x4364 ? BCM4364_FW_NAME :
+	    (sc->chiprev <= 7) ? BRCMF_FW_NAME_C2 : BRCMF_FW_NAME;
 
 	fw = firmware_get(fwname);
 	if (fw == NULL) {
@@ -1196,25 +1350,37 @@
 
 	{
 		const char *nvname;
-		nvname = (sc->chiprev <= 7) ? BRCMF_NVRAM_NAME_C2 :
+		nvname = sc->chip == 0x4364 ? BCM4364_NVRAM_NAME :
+		    (sc->chiprev <= 7) ? BRCMF_NVRAM_NAME_C2 :
 		    BRCMF_NVRAM_NAME;
 		nvram = firmware_get(nvname);
 		if (nvram != NULL) {
 			sc->nvram = brcmf_nvram_parse(nvram->data,
 			    nvram->datasize, &sc->nvram_len);
+			device_printf(dev, "loaded NVRAM %s (%u bytes parsed)\n",
+			    nvname, sc->nvram_len);
 			firmware_put(nvram, FIRMWARE_UNLOAD);
 		}
 	}
-
 	error = brcmf_download_fw(sc, fw);
 	firmware_put(fw, FIRMWARE_UNLOAD);
 
 	if (error != 0)
 		goto fail;
+	if (sc->chip == 0x4364) {
+		brcmf_load_blob(sc, "brcmfmac4364b3-pcie.clm_blob",
+		    "clmload", "clmload_status");
+		brcmf_load_blob(sc, "brcmfmac4364b3-pcie.txcap_blob",
+		    "txcapload", "txcapload_status");
+	}
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 transport initialized; configuring net80211\n");
 
 	error = brcmf_cfg_attach(sc);
 	if (error != 0)
 		goto fail;
+	if (sc->chip == 0x4364)
+		device_printf(sc->dev, "BCM4364 net80211 attached\n");
 
 	return (0);
 
@@ -1298,6 +1464,10 @@
 
 	brcmf_cfg_detach(sc);
 	brcmf_pcie_free_irq(sc);
+	if (sc->chip == 0x4364) {
+		pci_disable_busmaster(dev);
+		DELAY(1000);
+	}
 	sc->bus_ops->cleanup(sc);
 	brcmf_pcie_free_rings(sc);
 	mtx_destroy(&sc->ioctl_mtx);
