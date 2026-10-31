PORTNAME=	daemonarchy-pkg
DISTVERSION=	0.1.0
CATEGORIES=	ports-mgmt

MAINTAINER=	jkh@FreeBSD.org
COMMENT=	Colorful graphical package manager for Daemonarchy
WWW=		https://github.com/jordanhubbard/daemonarchy

LICENSE=	MIT

RUN_DEPENDS=	sudo:security/sudo \
		${PYTHON_PKGNAMEPREFIX}pygobject>0:devel/py-pygobject@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}sqlite3>0:databases/py-sqlite3@${PY_FLAVOR} \
		${LOCALBASE}/lib/girepository-1.0/Gtk-4.0.typelib:x11-toolkits/gtk40

USES=		python:run shebangfix
USE_GITHUB=	yes
GH_ACCOUNT=	jordanhubbard
GH_PROJECT=	daemonarchy
# Pin the reviewed commit of jordanhubbard/daemonarchy that carries apps/daemonarchy-pkg and the installer
# engine files it shares (parallel-fetch, progress-model).
GH_TAGNAME=	2f9b7ecf4c3bcdb4208198d12c63ee539f5e441d

NO_ARCH=	yes
NO_BUILD=	yes

APP=		apps/daemonarchy-pkg
ENGINE=		overlay/usr/local/libexec/daemonarchy-installer
LIBDIR=		lib/daemonarchy-pkg
MODULES=	__init__ catalogue cli engine plan repos theme transaction txview viewmodel window

SHEBANG_FILES=	${APP}/bin/daemonarchy-pkg ${APP}/bin/daemonarchy-pkg-helper ${ENGINE}/parallel-fetch

PLIST_FILES=	bin/daemonarchy-pkg \
		bin/daemonarchy-pkg-helper \
		${LIBDIR}/engine/parallel-fetch \
		${LIBDIR}/engine/progress-model.py \
		share/applications/daemonarchy-pkg.desktop
.for m in ${MODULES}
PLIST_FILES+=	${LIBDIR}/daemonarchy_pkg/${m}.py
.endfor

# The window and its logic are the module directory in lib/daemonarchy-pkg; the two launchers are in bin (the
# helper is the program that runs as root, so it lives in root-owned ${PREFIX}/bin, where the window looks for it
# first); the installer's plan fetcher and progress model are copied into lib/daemonarchy-pkg/engine, the same
# files the installer runs, so a plan and its events mean the same here as there.
do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/${LIBDIR}/engine ${STAGEDIR}${PREFIX}/${LIBDIR}/daemonarchy_pkg
.for m in ${MODULES}
	${INSTALL_DATA} ${WRKSRC}/${APP}/daemonarchy_pkg/${m}.py ${STAGEDIR}${PREFIX}/${LIBDIR}/daemonarchy_pkg
.endfor
	${INSTALL_SCRIPT} ${WRKSRC}/${APP}/bin/daemonarchy-pkg ${WRKSRC}/${APP}/bin/daemonarchy-pkg-helper \
		${STAGEDIR}${PREFIX}/bin
	${INSTALL_SCRIPT} ${WRKSRC}/${ENGINE}/parallel-fetch ${STAGEDIR}${PREFIX}/${LIBDIR}/engine
	${INSTALL_DATA} ${WRKSRC}/${ENGINE}/progress-model.py ${STAGEDIR}${PREFIX}/${LIBDIR}/engine
	${INSTALL_DATA} ${WRKSRC}/${APP}/share/applications/daemonarchy-pkg.desktop \
		${STAGEDIR}${PREFIX}/share/applications

.include <bsd.port.mk>
