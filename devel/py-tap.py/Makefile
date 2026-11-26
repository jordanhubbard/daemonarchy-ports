PORTNAME=	tap-py
DISTVERSION=	3.2.1
CATEGORIES=	devel python
MASTER_SITES=	PYPI
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}
DISTNAME=	tap_py-${DISTVERSION}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Test Anything Protocol tools for Python
WWW=		https://github.com/python-tap/tappy

LICENSE=	BSD2CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}hatchling>=0:devel/py-hatchling@${PY_FLAVOR}
TEST_DEPENDS=	${PYTHON_PKGNAMEPREFIX}more-itertools>=10.6.0:devel/py-more-itertools@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pyyaml>=6.0.2:devel/py-pyyaml@${PY_FLAVOR}

USES=		python:3.9+
USE_PYTHON=	autoplist concurrent pep517 pytest

NO_ARCH=	yes

OPTIONS_DEFINE=	YAML

YAML_DESC=	TAP version 13 YAML diagnostics
YAML_RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}more-itertools>=0:devel/py-more-itertools@${PY_FLAVOR} \
			${PYTHON_PKGNAMEPREFIX}pyyaml>=5.1:devel/py-pyyaml@${PY_FLAVOR}

# Keep the patch backup outside the module tree packaged by Hatch.
post-patch:
	${MV} ${WRKSRC}/tap/parser.py.orig ${WRKDIR}/tap-parser.py.orig

.include <bsd.port.mk>
