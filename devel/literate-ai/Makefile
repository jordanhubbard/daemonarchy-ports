PORTNAME=	literate-ai
DISTVERSION=	1.4.0
CATEGORIES=	devel python

MAINTAINER=	jkh@FreeBSD.org
COMMENT=	Specification-led software lifecycle harness (litai)
WWW=		https://github.com/jordanhubbard/literate-ai

LICENSE=	APACHE20
LICENSE_FILE=	${WRKSRC}/LICENSE

# The runtime dependencies are the exact packages pyproject.toml lists (the cyclonedx-python-lib[json-validation]
# extra is expanded, because a port cannot select another port's options). Version bounds are floors, not the
# upstream == pins: the ports tree carries one version of each library and pip's lock is not authority here.
# cryptography is an existing binary port, so building literate-ai never needs a Rust toolchain.
BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}setuptools>=63.1.0:devel/py-setuptools@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}wheel>0:devel/py-wheel@${PY_FLAVOR}
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}certifi>=2026.7.22:security/py-certifi@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}cryptography>=50.0.1:security/py-cryptography@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pyjwt>=2.10.0:www/py-pyjwt@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pyyaml>=6.0.3:devel/py-pyyaml@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}cyclonedx-python-lib>=11.11.0:textproc/py-cyclonedx-python-lib@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}jsonschema>=4.26.0:devel/py-jsonschema@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}referencing>=0.28.4:devel/py-referencing@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}fqdn>=0:www/py-fqdn@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}idna>=0:dns/py-idna@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}isoduration>=0:devel/py-isoduration@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}jsonpointer>=1.13:devel/py-jsonpointer@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}rfc3339-validator>=0:devel/py-rfc3339-validator@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}rfc3986-validator>=0.1.0:devel/py-rfc3986-validator@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}rfc3987-syntax>=1.1.0:www/py-rfc3987-syntax@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}uri-template>=0:net/py-uri-template@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}webcolors>=24.6.0:graphics/py-webcolors@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}packaging>=26.3:devel/py-packaging@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}semantic-version>=2.10.0:devel/py-semantic-version@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}univers>=32.0.1:devel/py-univers@${PY_FLAVOR}

USES=		python:3.11+
USE_PYTHON=	autoplist pep517
USE_GITHUB=	yes
GH_ACCOUNT=	jordanhubbard
# Pinned to the merged main commit 7854ff5d7 (PR #271, distribution version 1.4.0); v1.4.0 is not tagged upstream
# yet, so move this to the tag when it exists.
GH_TAGNAME=	7854ff5d766e54e71acc18aa8355a678ee0d1721

NO_ARCH=	yes

# literate-ai's PEP 517 backend refuses to build without an exact origin and revision, and a GitHub tarball has no
# .git directory to read them from; it embeds them so the installed `litai --version` names the source it came from.
MAKE_ENV=	LITAI_BUILD_REPOSITORY_URL=https://github.com/${GH_ACCOUNT}/${GH_PROJECT} \
		LITAI_BUILD_GIT_REVISION=${GH_TAGNAME}

OPTIONS_DEFINE=	MCP
OPTIONS_DEFAULT=MCP
MCP_DESC=	litai-mcp server (Model Context Protocol)
MCP_RUN_DEPENDS=${PYTHON_PKGNAMEPREFIX}mcp>=2.1.1:misc/py-mcp@${PY_FLAVOR}

# The distribution wheel's metadata asks for setuptools==83.0.0 and uses PEP 639 license fields that the ports
# setuptools does not understand; files/patch-pyproject.toml relaxes exactly those lines.

.include <bsd.port.mk>
