PORTNAME=	aether
DISTVERSIONPREFIX=	v
DISTVERSION=	4.30.0
PORTREVISION=	1
CATEGORIES=	x11-themes
MASTER_SITES+=	https://github.com/jordanhubbard/daemonarchy-ports/releases/download/${PORTNAME}-${DISTVERSION}-wasi-npm-cache-r1/:npmcache
DISTFILES+=	${PORTNAME}-${DISTVERSION}-wasi-npm-cache-r1.tar.gz:npmcache

MAINTAINER=	jkh@FreeBSD.org
COMMENT=	Desktop theming application that extracts colors from wallpapers
WWW=		https://github.com/omacom/aether

LICENSE=	MIT

BUILD_DEPENDS=	npm:www/npm
LIB_DEPENDS=	libharfbuzz.so:print/harfbuzz \
		libjavascriptcoregtk-4.1.so:www/webkit2-gtk@41 \
		libsoup-3.0.so:devel/libsoup3 \
		libwebkit2gtk-4.1.so:www/webkit2-gtk@41
RUN_DEPENDS=	xdg-mime:devel/xdg-utils

USES=		desktop-file-utils gnome go:modules pkgconfig
USE_GITHUB=	yes
GH_ACCOUNT=	omacom
USE_GNOME=	cairo gdkpixbuf glib20 gtk30 pango

GO_MOD_DIST=	github
GO_MODULE=	aether
GO_BUILDFLAGS=	-tags desktop,production,webkit2_41 \
		-ldflags "-s -X aether/cli.Version=${DISTVERSION}"

PLIST_FILES=	bin/aether \
		share/applications/aether.desktop \
		share/applications/li.oever.aether.url-handler.desktop \
		share/icons/hicolor/512x512/apps/aether.png \
		share/pixmaps/aether.png
PORTDOCS=	README.md

OPTIONS_DEFINE=	DOCS

# Build the embedded frontend offline with the same upstream WASI compiler
# backends on every architecture. The cache is bound to the patched lock file;
# no FreeBSD-specific Node addon binaries or prebuilt frontend are required.
# Select the WASI dependency context explicitly and refuse native fallbacks.

# Wails v2 only supports Linux, Darwin and Windows.  The vendored sources are
# patched to accept freebsd in their build constraints (files/patch-vendor_*),
# and the GOOS-suffixed Linux files are copied so they are built on FreeBSD.
WAILS_LINUX_FILES=	internal/frontend/desktop/desktop_linux.go \
			pkg/assetserver/webview/request_linux.go \
			pkg/assetserver/webview/responsewriter_linux.go \
			pkg/runtime/signal_linux.go

post-patch:
.for f in ${WAILS_LINUX_FILES}
	${CP} ${WRKSRC}/vendor/github.com/wailsapp/wails/v2/${f} \
		${WRKSRC}/vendor/github.com/wailsapp/wails/v2/${f:S/_linux/_freebsd/}
.endfor

pre-build:
	cd ${WRKSRC}/frontend && ${SETENV} ${MAKE_ENV} HOME=${WRKDIR} \
		npm ci --cpu=wasm32 --offline --cache ${WRKDIR}/npm-cache --ignore-scripts \
		--no-audit --no-fund --no-update-notifier && \
		${SETENV} ${MAKE_ENV} HOME=${WRKDIR} NAPI_RS_FORCE_WASI=error \
		node ${FILESDIR}/select-wasm-bindings.cjs ${WRKSRC}/frontend && \
		${SETENV} ${MAKE_ENV} HOME=${WRKDIR} NAPI_RS_FORCE_WASI=error npm run build

post-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/share/applications
	${INSTALL_DATA} ${WRKSRC}/build/linux/aether.desktop \
		${WRKSRC}/li.oever.aether.url-handler.desktop \
		${STAGEDIR}${PREFIX}/share/applications
	${MKDIR} ${STAGEDIR}${PREFIX}/share/pixmaps \
		${STAGEDIR}${PREFIX}/share/icons/hicolor/512x512/apps
	${INSTALL_DATA} ${WRKSRC}/icon.png \
		${STAGEDIR}${PREFIX}/share/pixmaps/aether.png
	${INSTALL_DATA} ${WRKSRC}/assets/aether-icon-512.png \
		${STAGEDIR}${PREFIX}/share/icons/hicolor/512x512/apps/aether.png

post-install-DOCS-on:
	${MKDIR} ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/README.md ${STAGEDIR}${DOCSDIR}

.include <bsd.port.mk>
