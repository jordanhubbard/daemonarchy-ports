PORTNAME=	drm-612-kmod
DISTVERSION=	${DRM_KMOD_DISTVERSION}
PORTREVISION=	8
CATEGORIES=	graphics kld

.include "Makefile.version"

MAINTAINER=	x11@FreeBSD.org
COMMENT=	Direct Rendering Manager (DRM) GPU drivers
WWW=		https://github.com/freebsd/drm-kmod/

LICENSE=	BSD2CLAUSE MIT GPLv2
LICENSE_COMB=	multi

ONLY_FOR_ARCHS=	amd64 aarch64
ONLY_FOR_ARCHS_REASON=	the full KMS set is supported on amd64; aarch64 carries VirtIO and optional EFI framebuffer KMS

CONFLICTS_INSTALL=	drm-latest-kmod \
			drm-515-kmod \
			drm-61-kmod \
			drm-66-kmod

USES=		kmod uidfix compiler:c++11-lang

USE_GITHUB=	yes
GH_ACCOUNT=	freebsd
GH_PROJECT=	drm-kmod
GH_TAGNAME=	${DRM_KMOD_GH_TAGNAME}

OPTIONS_DEFINE_aarch64=	EFIFB
OPTIONS_DEFAULT_aarch64=	EFIFB
EFIFB_DESC=		Build opt-in software KMS for the EFI framebuffer
EFIFB_MAKE_ARGS=		WITH_EFIFB_DRM=1

.include <bsd.port.options.mk>

# Preserve the amd64 source/driver selection. ARM additionally supplies the
# firmware display control; building it does not enable or load the driver.
.if ${ARCH} == amd64
PLIST_SUB+=	FULLKMS="" EFIFB="@comment "
.else
PLIST_SUB+=	FULLKMS="@comment "
_ARM_KMODS=	dmabuf ttm drm virtio
.if !empty(PORT_OPTIONS:MEFIFB)
_ARM_KMODS+=	efifb
PLIST_SUB+=	EFIFB=""
.else
PLIST_SUB+=	EFIFB="@comment "
.endif
MAKE_ARGS+=	KMODS="${_ARM_KMODS}"
# Software-rendered desktops (Mesa kms_swrast) create dumb buffers on the render node:
EXTRA_PATCHES+=	${FILESDIR}/extra-patch-drm-ioctl-dumb-render
.endif

SUB_FILES=	pkg-message
SUB_LIST=	PORTNAME=${PORTNAME} OPSYS=${OPSYS} OSREL=${OSREL}

.if ${OPSYS} != FreeBSD
IGNORE=         not supported on anything but FreeBSD (missing linuxkpi functionality)
.elif ${OSVERSION} < 1500509
IGNORE=         not supported on FreeBSD < 1500509, no kernel support
.elif 1600000 <= ${OSVERSION} && ${OSVERSION} < 1600018
IGNORE=         not supported on FreeBSD 16 before 1600018, no kernel support
.endif

MAKE_ENV+=	MAKEOBJDIRPREFIX=${WRKSRC}/obj

# EXTRA_PATCHES runs before the normal patches. This incremental patch needs
# the retained VirtIO/shmem implementation, so apply it afterward instead.
post-patch-EFIFB-on:
	${PATCH} -d ${WRKSRC} ${PATCH_ARGS} --fuzz=0 < ${FILESDIR}/extra-patch-efifb-drm

pre-build:
		${MKDIR} ${WRKSRC}/obj
		(cd ${WRKSRC} ; ${SETENV} ${MAKE_ENV} ${MAKE_CMD} ${MAKE_ARGS} obj)

.if !empty(PORT_OPTIONS:MEFIFB)
do-test:
	${CC} ${CFLAGS} -std=c11 -Wall -Wextra -Werror \
		${WRKSRC}/tests/test_efi_fb_layout.c -o ${WRKDIR}/test-efi-fb-layout
	${WRKDIR}/test-efi-fb-layout
.endif

.include <bsd.port.mk>
