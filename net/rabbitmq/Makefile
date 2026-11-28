PORTNAME=	rabbitmq
DISTVERSION=	4.4.1
PORTREVISION=	1
CATEGORIES=	net
MASTER_SITES=	https://github.com/${PORTNAME}/${PORTNAME}-server/releases/download/v${DISTVERSION}/
DISTNAME=	${PORTNAME}-server-${DISTVERSION}
DISTFILES=	${DISTNAME}${EXTRACT_SUFX}

MAINTAINER=	erlang@FreeBSD.org
# Co-maintainers: vvd@FreeBSD.org fsbruva@yahoo.com
COMMENT=	Erlang implementation of AMQP
WWW=		https://www.rabbitmq.com/

LICENSE=	MPL20
LICENSE_FILE=	${WRKSRC}/LICENSE-MPL-RabbitMQ

BUILD_DEPENDS=	7zz:archivers/7-zip \
		elixir>=1.17:lang/elixir \
		erlang-runtime${_ERLANG_VER}>=${_ERLANG_VER}:lang/erlang-runtime${_ERLANG_VER} \
		git:devel/git@lite \
		rsync:net/rsync
RUN_DEPENDS=	erlang-runtime${_ERLANG_VER}>=${_ERLANG_VER}:lang/erlang-runtime${_ERLANG_VER}

FLAVORS=	er28 er29 er27
FLAVOR?=	${FLAVORS:[1]}
${FLAVOR}_PKGNAMESUFFIX=	${FLAVOR:N${FLAVORS:[1]}:C|(.+)|-\1|}

USES=		cpe gmake python:build tar:xz
CPE_VENDOR=	pivotal_software
USE_GITHUB=	nodefault
# The publisher source distribution bundles external dependencies but omits
# tests. Retain the complete matching upstream checkout as well.
GH_TUPLE=	rabbitmq:rabbitmq-server:aa914c53d2f40f4be3d3a8738d0d5018ea1537e4:original
USE_RC_SUBR=	${PORTNAME}

CONFLICTS_INSTALL=	${PORTNAME}*

NO_ARCH=	yes
SUB_LIST=	_ERLANG_LIB=${_ERLANG_LIB}

MAKE_ENV=	HEX_OFFLINE=true REBAR_OFFLINE=1 \
		PROJECT_VERSION=${DISTVERSION} RABBITMQ_VERSION=${DISTVERSION} \
		PATH=${LOCALBASE}/lib/${_ERLANG_LIB}/bin:${PATH}
MAKE_ARGS=	PYTHON=${PYTHON_CMD} ESCRIPT_ZIP="7zz a -tzip -mx=9 -mtc=off"

_DIR=		${PORTNAME}_server-${DISTVERSION}
WRKSRC=		${WRKDIR}/${DISTNAME}

USERS=		${PORTNAME}
GROUPS=		${PORTNAME}

PLIST_SUB=	VERSION=${DISTVERSION}

_ERLANG_VER=	${FLAVOR:S|er||}
_ERLANG_LIB=	erlang${_ERLANG_VER}

_SCRIPTS=	rabbitmq-defaults rabbitmq-diagnostics rabbitmq-env rabbitmq-plugins \
		rabbitmq-queues rabbitmq-server rabbitmq-streams rabbitmq-upgrade rabbitmqctl vmware-rabbitmq

post-extract:
	# Overlay all original checkout files and tests, retaining the bundled
	# external source directories from the publisher distribution.
	${CP} -Rp ${WRKSRC_original}/. ${WRKSRC}

post-patch:
	@${REINPLACE_CMD} -i '' -e 's|%%LOCALBASE%%|${LOCALBASE}|' \
		-e 's|%%_ERLANG_LIB%%|${_ERLANG_LIB}|' \
		${WRKSRC}/deps/rabbit/scripts/rabbitmq-defaults

do-build:
	(cd ${WRKSRC} && ${SETENV} ${MAKE_ENV} ${LOCALBASE}/bin/elixir -e \
		'unless Version.match?(System.version(), ">= 1.17.0 and < 1.21.0"), do: raise("RabbitMQ requires Elixir >= 1.17 and < 1.21")')
	# The CLI needs rabbit_common's generated headers before the parallel
	# top-level build starts.
	(cd ${WRKSRC} && ${SETENV} ${MAKE_ENV} ${MAKE_CMD} ${_MAKE_JOBS} ${MAKE_ARGS} deps)
	(cd ${WRKSRC} && ${SETENV} ${MAKE_ENV} ${MAKE_CMD} ${_MAKE_JOBS} ${MAKE_ARGS} all)
	(cd ${WRKSRC} && ${SETENV} ${MAKE_ENV} ${MAKE_CMD} ${_MAKE_JOBS} ${MAKE_ARGS} dist)
	# The ports framework invokes GNU make recursively, so upstream's
	# MAKELEVEL=0 shortcut does not generate the command wrappers for us.
	(cd ${WRKSRC} && ${SETENV} ${MAKE_ENV} ${MAKE_CMD} ${_MAKE_JOBS} ${MAKE_ARGS} install-cli)
	@${REINPLACE_CMD} -i '' -e 's|/usr/bin/env escript|${LOCALBASE}/lib/${_ERLANG_LIB}/bin/escript|' \
		${WRKSRC}/escript/*

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/lib/${_DIR}/sbin
	(cd ${WRKSRC} && \
		${COPYTREE_BIN} escript ${STAGEDIR}${PREFIX}/lib/${_DIR} && \
		${COPYTREE_SHARE} plugins      ${STAGEDIR}${PREFIX}/lib/${_DIR})
.for _file in ${_SCRIPTS}
	${INSTALL_SCRIPT} ${WRKSRC}/sbin/${_file} ${STAGEDIR}${PREFIX}/lib/${_DIR}/sbin
.endfor
	${ECHO_CMD} "Put your EZs here and use rabbitmq-plugins to enable them." \
		> ${STAGEDIR}${PREFIX}/lib/${_DIR}/plugins/README

post-install:
	${MKDIR} ${STAGEDIR}${ETCDIR} \
		 ${STAGEDIR}/var/db/${PORTNAME}/mnesia \
		 ${STAGEDIR}/var/log/${PORTNAME}
	# ${INSTALL_DATA} ${WRKSRC}/deps/rabbit/docs/advanced.config.example \
	# ${STAGEDIR}${ETCDIR}/advanced.config.sample
	# ${INSTALL_DATA} ${WRKSRC}/deps/rabbit/docs/rabbitmq.conf.example \
	# ${STAGEDIR}${ETCDIR}/rabbitmq.conf.sample
.for _file in ${_SCRIPTS}
	${RLN} ${STAGEDIR}${PREFIX}/lib/${_DIR}/sbin/${_file} ${STAGEDIR}${PREFIX}/sbin
.endfor
	${INSTALL_MAN} ${WRKSRC}/deps/rabbit/docs/*.5 ${STAGEDIR}${PREFIX}/share/man/man5
	${INSTALL_MAN} ${WRKSRC}/deps/rabbit/docs/*.8 ${STAGEDIR}${PREFIX}/share/man/man8
	# ${INSTALL} ${WRKSRC}/plugins/rabbitmq_management-*/priv/www/cli/rabbitmqadmin ${STAGEDIR}${PREFIX}/bin

.include <bsd.port.mk>
