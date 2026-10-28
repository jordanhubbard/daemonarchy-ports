PORTNAME=	t2-touchid
DISTVERSION=	0.1.0-20261028
CATEGORIES=	security sysutils
DIST_SUBDIR=	${PORTNAME}

MAINTAINER=	jkh@FreeBSD.org
COMMENT=	Apple T2 Touch ID bridge for fprintd
WWW=		https://github.com/kaiT2en/KaiT2en-Fedora/tree/main/t2-services/t2-touchid

LICENSE=	GPLv3+
LICENSE_FILE=	${WRKSRC}/t2-touchid/LICENSE

ONLY_FOR_ARCHS=	amd64
ONLY_FOR_ARCHS_REASON=	the Apple T2 controller exists only in Intel Macs

RUN_DEPENDS=	fprintd>0:security/fprintd

USES=		cargo
USE_GITHUB=	yes
GH_ACCOUNT=	kaiT2en
GH_PROJECT=	KaiT2en-Fedora
# Upstream has no releases; pin the exact commit (t2-touchid 0.1.0).
GH_TAGNAME=	16b58080ebc8542d7b11486a45722350f277ed33
USE_RC_SUBR=	t2_touchid

# t2-touchid depends on ../shared/protocols/t2-bridgexpc by path, so the
# source root is t2-services and the cargo project one level down.
WRKSRC_SUBDIR=		t2-services
CARGO_SRC_SUBDIR=	t2-touchid

PLIST_FILES=	bin/t2-touchid \
		share/dbus-1/system.d/org.kait2en.TouchId.conf
PORTDOCS=	LICENSE README.md

OPTIONS_DEFINE=	DOCS

# With CARGO_SRC_SUBDIR set, USES=cargo leaves build, install and test to
# the port.
do-build:
	@${CARGO_CARGO_RUN} build \
		--manifest-path ${CARGO_CARGOTOML} \
		--verbose \
		${CARGO_BUILD_ARGS}

do-install:
	${INSTALL_PROGRAM} ${CARGO_TARGET_DIR}/release/t2-touchid \
		${STAGEDIR}${PREFIX}/bin
	@${MKDIR} ${STAGEDIR}${PREFIX}/share/dbus-1/system.d
	${INSTALL_DATA} ${CARGO_WRKSRC}/integration/dbus/org.kait2en.TouchId.conf \
		${STAGEDIR}${PREFIX}/share/dbus-1/system.d

post-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${PORTDOCS:S|^|${CARGO_WRKSRC}/|} ${STAGEDIR}${DOCSDIR}

do-test:
	@${CARGO_CARGO_RUN} test \
		--manifest-path ${CARGO_CARGOTOML} \
		${CARGO_TEST_ARGS}
	@${CARGO_CARGO_RUN} test \
		--manifest-path ${CARGO_CARGOTOML} \
		--package t2-bridgexpc \
		${CARGO_TEST_ARGS}

.include <bsd.port.mk>
