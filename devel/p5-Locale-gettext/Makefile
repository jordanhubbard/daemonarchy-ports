PORTNAME=	Locale-gettext
PORTVERSION=	1.07
CATEGORIES=	devel perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	demon@FreeBSD.org
COMMENT=	Message handling functions
WWW=		https://metacpan.org/release/gettext

LICENSE=	ARTPERL10 GPLv2
LICENSE_COMB=	dual

WRKSRC=		${WRKDIR}/${PORTNAME}-${PORTVERSION}

CONFIGURE_ENV+=	LOCALBASE=${LOCALBASE}
USES=		gettext perl5
USE_PERL5=	configure

# gettext suppresses translations in C.UTF-8; keep the translation tests active.
TEST_ENV=	${MAKE_ENV} LC_ALL=en_US.UTF-8

post-install:
	${STRIP_CMD} ${STAGEDIR}${PREFIX}/${SITE_ARCH_REL}/auto/Locale/gettext/gettext.so

.include <bsd.port.mk>
