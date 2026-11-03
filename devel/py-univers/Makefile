PORTNAME=	univers
DISTVERSION=	32.0.1
CATEGORIES=	devel python
MASTER_SITES=	PYPI
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	jkh@FreeBSD.org
COMMENT=	Parse and compare software package versions and version ranges
WWW=		https://github.com/aboutcode-org/univers

LICENSE=	APACHE20 BSD3CLAUSE MIT
LICENSE_COMB=	multi
LICENSE_FILE_APACHE20=	${WRKSRC}/apache-2.0.LICENSE
LICENSE_FILE_BSD3CLAUSE=${WRKSRC}/bsd-new.LICENSE
LICENSE_FILE_MIT=	${WRKSRC}/mit.LICENSE

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}setuptools-scm>=6:devel/py-setuptools-scm@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}wheel>0:devel/py-wheel@${PY_FLAVOR}
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}attrs>0:devel/py-attrs@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}packaging>0:devel/py-packaging@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}semantic-version>0:devel/py-semantic-version@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}semver>0:devel/py-semver@${PY_FLAVOR}

USES=		python
USE_PYTHON=	autoplist concurrent pep517

NO_ARCH=	yes

.include <bsd.port.mk>
