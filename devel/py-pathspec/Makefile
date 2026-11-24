PORTNAME=	pathspec
PORTVERSION=	1.1.1
PORTREVISION=	1
CATEGORIES=	devel python
MASTER_SITES=	PYPI
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

PATCH_SITES=	https://github.com/cpburnz/python-pathspec/commit/
PATCHFILES=	f404ca47871a9f2f854bbadc5cb3d77155f64db3.patch:-p1

MAINTAINER=	matthew@FreeBSD.org
COMMENT=	Utility library for pattern matching of file paths
WWW=		https://pypi.org/project/pathspec/

LICENSE=	MPL20
LICENSE_FILE=	${WRKSRC}/LICENSE

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}flit-core>=3.2<5:devel/py-flit-core@${PY_FLAVOR}

USES=		python
USE_PYTHON=	autoplist concurrent pep517 pytest

NO_ARCH=	yes

.include <bsd.port.mk>
