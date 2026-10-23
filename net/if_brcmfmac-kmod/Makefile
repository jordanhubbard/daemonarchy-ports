PORTNAME=	if_brcmfmac
DISTVERSION=	20260402
PORTREVISION=	1
CATEGORIES=	net kld
PKGNAMESUFFIX=	-kmod

MAINTAINER=	jkh@FreeBSD.org
COMMENT=	Native Broadcom FullMAC Wi-Fi kernel module
WWW=		https://github.com/narqo/freebsd-brcmfmac

LICENSE=	ISCL
LICENSE_FILE=	${WRKSRC}/LICENSE

ONLY_FOR_ARCHS=	amd64
USES=		kmod uidfix
USE_GITHUB=	yes
GH_ACCOUNT=	narqo
GH_PROJECT=	freebsd-brcmfmac
GH_TAGNAME=	9adbe774d3be91827d628fbd811daf63512acb6d

PLIST_FILES=	${KMODDIR}/if_brcmfmac.ko

.include <bsd.port.mk>
