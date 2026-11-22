PORTNAME=	if_brcmfmac
DISTVERSION=	20260402
PORTREVISION=	4
CATEGORIES=	net kld
PKGNAMESUFFIX=	-kmod

MAINTAINER=	jkh@FreeBSD.org
COMMENT=	Native Broadcom FullMAC Wi-Fi kernel module
WWW=		https://github.com/narqo/freebsd-brcmfmac

LICENSE=	ISCL
LICENSE_FILE=	${WRKSRC}/LICENSE

ONLY_FOR_ARCHS=	aarch64 amd64
USES=		kmod uidfix
USE_GITHUB=	yes
GH_ACCOUNT=	narqo
GH_PROJECT=	freebsd-brcmfmac
GH_TAGNAME=	9adbe774d3be91827d628fbd811daf63512acb6d

PATCH_ARGS+=	--fuzz 0

# Keep CTF available to the kernel without adding split debug payloads.
MAKE_ARGS+=	WITH_CTF=1 DEBUG_FLAGS=-g MK_SPLIT_KERNEL_DEBUG=no
.if defined(KERNBUILDDIR) && !empty(KERNBUILDDIR)
MAKE_ARGS+=	KERNBUILDDIR="${KERNBUILDDIR}"
.endif

PLIST_FILES=	${KMODDIR}/if_brcmfmac.ko

post-install:
	${STRIP_CMD} ${STAGEDIR}${KMODDIR}/if_brcmfmac.ko

.include <bsd.port.mk>
