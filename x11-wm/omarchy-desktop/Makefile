PORTNAME=	omarchy-desktop
PORTVERSION=	4.0.4
CATEGORIES=	x11-wm
MASTER_SITES=	# empty
DISTFILES=	# empty
EXTRACT_ONLY=	# empty

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Omarchy desktop with its complete application set (meta-port)
WWW=		https://omarchy.org/

# The Omarchy runtime, the Hyprland session stack and the tools the desktop
# relies on. Arch packages without a FreeBSD equivalent (kernels, firmware,
# DKMS drivers, Limine/Snapper, NetworkManager, BlueZ, power-profiles-daemon)
# are covered by the base system or by the omarchy port's compat layer.
RUN_DEPENDS=	omarchy>0:x11-wm/omarchy \
		egl-wayland>0:graphics/egl-wayland \
		font-awesome>0:x11-fonts/font-awesome \
		fontconfig>0:x11-fonts/fontconfig \
		foot>0:x11/foot \
		gnome-themes-extra>0:x11-themes/gnome-themes-extra \
		grim>0:x11/grim \
		gtk4-layer-shell>0:x11-toolkits/gtk4-layer-shell \
		hyprland-guiutils>0:x11/hyprland-guiutils \
		hyprland-preview-share-picker>0:x11/hyprland-preview-share-picker \
		hyprpicker>0:x11/hyprpicker \
		hyprsunset>0:accessibility/hyprsunset \
		ia-writer-fonts>0:x11-fonts/ia-writer-fonts \
		libsecret>0:security/libsecret \
		mesa-dri>0:graphics/mesa-dri \
		node>0:www/node \
		noto>0:x11-fonts/noto \
		pamixer>0:audio/pamixer \
		pipewire-spa-oss>0:audio/pipewire-spa-oss \
		pulseaudio>0:audio/pulseaudio \
		${PYTHON_PKGNAMEPREFIX}pygobject>0:devel/py-pygobject@${PY_FLAVOR} \
		qt6-imageformats>0:graphics/qt6-imageformats \
		qt6-wayland>0:graphics/qt6-wayland \
		sddm>0:x11/sddm \
		slurp>0:x11/slurp \
		tensaku>0:graphics/tensaku \
		udiskie>0:sysutils/udiskie \
		bsdisks>0:sysutils/bsdisks \
		webp-pixbuf-loader>0:graphics/webp-pixbuf-loader \
		wl-clipboard>0:x11/wl-clipboard \
		wtype>0:x11/wtype \
		xdg-desktop-portal>0:deskutils/xdg-desktop-portal \
		xdg-desktop-portal-gtk>0:x11/xdg-desktop-portal-gtk \
		yaru-icon-theme>0:x11-themes/yaru-icon-theme

USES=		metaport python:run

OPTIONS_DEFINE=		APPS CLI DEV INPUT OMAAPPS PRINT VAAPI
OPTIONS_DEFAULT=	APPS CLI DEV INPUT OMAAPPS PRINT VAAPI

APPS_DESC=	Default applications (browser, office, media, notes)
CLI_DESC=	Terminal tools Omarchy's shell setup expects
DEV_DESC=	Development languages, database clients and container tools
INPUT_DESC=	Fcitx5 input method (XCompose and CJK input)
OMAAPPS_DESC=	Omarchy's own applications (Aether, Omacalc, Omasnap, ...)
PRINT_DESC=	Printing and network service discovery
VAAPI_DESC=	Hardware video decoding drivers

APPS_RUN_DEPENDS=	chromium>0:www/chromium \
			evince>0:graphics/evince \
			ffmpegthumbnailer>0:multimedia/ffmpegthumbnailer \
			gvfs>0:filesystems/gvfs \
			ImageMagick7>0:graphics/ImageMagick7 \
			imv>0:graphics/imv \
			kdenlive>0:multimedia/kdenlive \
			libreoffice>0:editors/libreoffice \
			moonlight-qt>0:games/moonlight-qt \
			mpv>0:multimedia/mpv \
			mpv-mpris>0:multimedia/mpv-mpris \
			nautilus>0:x11-fm/nautilus \
			nautilus-python>0:x11-fm/nautilus-python \
			obs-studio>0:multimedia/obs-studio \
			obsidian>0:textproc/obsidian \
			pinta>0:graphics/pinta \
			sushi>0:x11-fm/sushi \
			tesseract>0:graphics/tesseract \
			tesseract-data>0:graphics/tesseract-data \
			vips>0:graphics/vips \
			xournalpp>0:graphics/xournalpp \
			yt-dlp>0:www/yt-dlp \
			zbar>0:graphics/zbar
CLI_RUN_DEPENDS=	bash-completion>0:shells/bash-completion \
			bat>0:textproc/bat \
			btop>0:sysutils/btop \
			dua-cli>0:sysutils/dua-cli \
			exfat-utils>0:filesystems/exfat-utils \
			eza>0:sysutils/eza \
			fastfetch>0:sysutils/fastfetch \
			fd-find>0:sysutils/fd \
			fzf>0:textproc/fzf \
			inotify-tools>0:sysutils/inotify-tools \
			inxi>0:sysutils/inxi \
			lazygit>0:devel/lazygit \
			libqrencode>0:graphics/libqrencode \
			neovim>0:editors/neovim \
			omarchy-nvim>0:editors/omarchy-nvim \
			plocate>0:sysutils/plocate \
			ripgrep>0:textproc/ripgrep \
			socat>0:net/socat \
			starship>0:shells/starship \
			tldr>0:sysutils/tldr \
			tmux>0:sysutils/tmux \
			tobi-try>0:devel/tobi-try \
			tzupdate>0:sysutils/tzupdate \
			usage>0:devel/usage \
			whois>0:net/whois \
			zoxide>0:sysutils/zoxide
DEV_USES=		lua:51,run ruby:run
DEV_RUN_DEPENDS=	autoconf-archive>0:devel/autoconf-archive \
			docker-buildx>0:sysutils/docker-buildx \
			docker-compose>0:sysutils/docker-compose \
			dotnet>0:lang/dotnet \
			fakeroot>0:security/fakeroot \
			lazydocker>0:sysutils/lazydocker \
			libyaml>0:textproc/libyaml \
			llvm>0:devel/llvm \
			${LUA_PKGNAMEPREFIX}luarocks>0:devel/lua-luarocks@${LUA_FLAVOR} \
			mariadb118-client>0:databases/mariadb118-client \
			mise>0:sysutils/mise \
			postgresql18-client>0:databases/postgresql18-client \
			${PYTHON_PKGNAMEPREFIX}poetry-core>0:devel/py-poetry-core@${PY_FLAVOR} \
			tree-sitter-cli>0:devel/tree-sitter-cli
INPUT_RUN_DEPENDS=	fcitx5>0:textproc/fcitx5 \
			fcitx5-gtk3>0:textproc/fcitx5-gtk \
			fcitx5-qt6>0:textproc/fcitx5-qt
OMAAPPS_RUN_DEPENDS=	aether>0:x11-themes/aether \
			cliamp>0:audio/cliamp \
			herdr>0:sysutils/herdr \
			omacalc>0:math/omacalc \
			omacut>0:multimedia/omacut \
			omasnap>0:graphics/omasnap \
			omawrite>0:editors/omawrite \
			owe>0:x11/owe \
			owe-lockfeed>0:x11/owe-lockfeed \
			ttfx>0:misc/ttfx
PRINT_RUN_DEPENDS=	avahi>0:net/avahi \
			cups>0:print/cups \
			cups-filters>0:print/cups-filters \
			cups-pk-helper>0:print/cups-pk-helper \
			nss_mdns>0:dns/nss_mdns \
			system-config-printer>0:print/system-config-printer

.include <bsd.port.options.mk>

.if ${PORT_OPTIONS:MVAAPI} && (${ARCH} == amd64 || ${ARCH} == i386)
RUN_DEPENDS+=	libva-intel-driver>0:multimedia/libva-intel-driver \
		libva-intel-media-driver>0:multimedia/libva-intel-media-driver \
		libvpl>0:multimedia/libvpl
.endif

.include <bsd.port.mk>
