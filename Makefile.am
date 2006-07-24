SUBDIRS= m4 m4  lib src resources po
EXTRA_DIST = config.rpath  config.rpath  config.rpath  \
        README \
        COPYING \
        AUTHORS \
        ChangeLog \
        INSTALL \
        NEWS \
        TODO \
		build/dev-cpp/Makefile.win \
		build/dev-cpp/xmlTreeNav.dev \
		build/dev-cpp/xmlTreeNav.ico \
		build/dev-cpp/xmlTreeNav.layout \
		build/dev-cpp/xmlTreeNav_private.h \
		build/dev-cpp/xmlTreeNav_private.rc \
		build/dev-cpp/xmlTreeNav_private.res \
		build/vc/XmlTreeNav.dsp \
        build/vc/XmlTreeNav.dsw \
        build/win32/XmlTreeNav.iss \
        build/win32/make_zip.bat \
        build/debian/control \
        build/debian/changelog \
        build/debian/conffiles \
        build/debian/copyright \
        build/debian/menu \
        build/debian/postinst \
        build/debian/postrm \
        build/debian/rules \
        build/debian/xmltreenav.1 \
        build/debian/xmltreenav.xpm \
        po/po_build.bat \
        po/po_update.bat \
        doc/HOWTO-Package-Debian.txt \
		doc/xmlTreeNav_en.html
	

deb:
	ln -sf build/debian .
	chmod +x debian/rules
	dpkg-buildpackage -rfakeroot
	fakeroot debian/rules clean

ACLOCAL_AMFLAGS = -I m4
