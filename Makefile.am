SUBDIRS= lib src resources po doc
EXTRA_DIST = config.rpath m4/ChangeLog  \
        README \
        COPYING \
        AUTHORS \
        ChangeLog \
        INSTALL \
        NEWS \
        TODO \
		bin/config.xml \
		bin/filter-added-removed-or-equal.xsl \
        build/dev-cpp/xmlTreeNav.layout \
        build/dev-cpp/xmlTreeNav.dev \
        build/dev-cpp/xmlTreeNav_private.h \
        build/dev-cpp/xmlTreeNav_private.rc \
        build/dev-cpp/xmlTreeNav_private.res \
        build/dev-cpp/xmlTreeNav.ico \
        build/dev-cpp/Makefile.win \
        build/vc/XmlTreeNav.dsp \
        build/vc/.cvsignore \
        build/vc/XmlTreeNav.vcproj \
        build/vc/XmlTreeNav.dsw \
        build/vc/XmlTreeNav.sln \
        build/vc/_wx_intellisense.h \
        build/vc2010/xmlTreeNav.sln \
        build/vc2010/xmlTreeNav.vcxproj \
        build/vc2013/xmlTreeNav.sln \
        build/vc2013/xmlTreeNav.vcxproj.filters \
        build/vc2013/xmlTreeNav.vcxproj \
		build/vc2017/xmlTreeNav.sln \
		build/vc2017/xmlTreeNav.vcxproj.filters \
		build/vc2017/xmlTreeNav.vcxproj \
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
        po/extra-strings.cpp \
        doc/HOWTO-Package-Debian.txt
	

deb-old:
	ln -sf build/debian .
	chmod +x debian/rules
	dpkg-buildpackage -I*.a -I*.la -I*.o -I*.so -I.*.sw? -I*/*~ -I,,* -I.[#~]* -I.arch-ids -I.arch-inventory -I.be -I.bzr -I.bzr.backup -I.bzr.tags -I.bzrignore -I.cvsignore -I.deps -I.git -I.gitattributes -I.gitignore -I.gitmodules -I.hg -I.hgignore -I.hgsigs -I.hgtags -I.shelf -I.svn -ICVS -IDEADJOE -IRCS -I_MTN -I_darcs
	rm -Rf debian/$(PACKAGE)/

debarchiver:
	mv ../$(PACKAGE)_* /home/www/dist/debian/incoming
	debarchiver --ignoredestcheck --dl 4
	debarchiver --autoscanall
    
testapt:
	sudo apt-get update -o "Dir::Etc::sourcelist=sources.list.d/lprp.list" -o "Dir::Etc::sourceparts=-" -o "APT::Get::List-Cleanup=0"
	sudo apt-get install --reinstall xmltreenav


DEBSOURCEPKG=$(PACKAGE)_$(VERSION).orig.tar.gz
DEBSOURCEDIR=$(PACKAGE)-$(VERSION)
deb: dist
	ln -sf build/debian .
	scite debian/control
	scite debian/changelog
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
	debuild --no-tgz-check -rfakeroot -G -D ; \
	cd .. ; \
	#rm --force $(DEBSOURCEPKG); \
	rm --recursive --force $(DEBSOURCEDIR)
	aptly -force-replace=true repo add local $(PACKAGE)_$(VERSION)*.dsc $(PACKAGE)_$(VERSION)*.deb
	@echo "To publish :"
	@echo aptly -gpg-key="remi+debian@via.ecp.fr" -force-overwrite publish update stable filesystem:web:debian

ppa:
	scite debian/control
	scite debian/changelog
	debuild -S -sa
	dput ppa ../*.changes


ACLOCAL_AMFLAGS = -I m4
