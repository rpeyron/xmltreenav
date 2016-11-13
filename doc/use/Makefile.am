helpdir = $(pkgdatadir)
help_DATA = xmltreenav.htb

xmltreenav_htb_SRC = \
        help_en.html \
        contents.hhc \
        xmltreenav.hhp

EXTRA_DIST = $(xmltreenav_htb_SRC)

xmltreenav.htb: $(xmltreenav_htb_SRC)
	zip -9 -r -j xmltreenav.htb * -x xmltreenav.htb

clean:
	rm -f xmltreenav.htb


