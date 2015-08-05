SUBDIRS= lib src resources po
EXTRA_DIST = config.rpath m4/ChangeLog  config.rpath m4/ChangeLog  config.rpath  config.rpath  config.rpath  \
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
        build/vc/XmlTreeNav.vcproj \
        build/win32/XmlTreeNav.iss \
        build/win32/make_zip.bat \
        build/debian/control \
        build/debian/changelog \
        build/debian/conffiles \
        build/debian/copyright \
        build/debian/menu \
        build/debian/xmltreenav.desktop \
        build/debian/postinst \
        build/debian/postrm \
        build/debian/rules \
        build/debian/xmltreenav.1 \
        build/debian/xmltreenav.xpm \
        po/po_build.bat \
        po/po_update.bat \
        doc/HOWTO-Package-Debian.txt \
		doc/xmlTreeNav_en.html
	

old-deb:
	ln -sf build/debian .
	chmod +x debian/rules
	dpkg-buildpackage -rfakeroot
	fakeroot debian/rules clean


DEBSOURCEPKG=$(PACKAGE)_$(VERSION).orig.tar.gz
DEBSOURCEDIR=$(PACKAGE)-$(VERSION)
deb: dist
	ln -sf build/debian .
	@echo "Building Debian package..."; \
	if test ! -d debian; then \
		echo "The files needed for building a Debian package are not" \
		"included by default in the distribution. To build a package, check" \
		"out the project from source control."; \
		exit 1; \
	fi; \
	mv $(PACKAGE)-$(VERSION).tar.gz $(DEBSOURCEPKG) && \
	tar --extract --gunzip --file $(DEBSOURCEPKG) && \
	mkdir -p $(DEBSOURCEDIR)/debian && \
	cp \
		debian/* \
		$(DEBSOURCEDIR)/debian && \
	cd $(DEBSOURCEDIR) && \
	debuild --no-tgz-check -rfakeroot -D ; \
	cd .. ; \
	rm --force $(DEBSOURCEPKG); \
	rm --recursive --force $(DEBSOURCEDIR)

deb-src: dist
	ln -sf build/debian .
	@echo "Building Debian package..."; \
	if test ! -d debian; then \
		echo "The files needed for building a Debian package are not" \
		"included by default in the distribution. To build a package, check" \
		"out the project from source control."; \
		exit 1; \
	fi; \
	mv $(PACKAGE)-$(VERSION).tar.gz $(DEBSOURCEPKG) && \
	tar --extract --gunzip --file $(DEBSOURCEPKG) && \
	mkdir -p $(DEBSOURCEDIR)/debian && \
	cp \
		debian/* \
		$(DEBSOURCEDIR)/debian && \
	cd $(DEBSOURCEDIR) && \
	debuild --no-tgz-check -rfakeroot -S -sa  ; \
	cd ..; \
	rm --force $(DEBSOURCEPKG); \
	rm --recursive --force $(DEBSOURCEDIR)


ACLOCAL_AMFLAGS = -I m4
