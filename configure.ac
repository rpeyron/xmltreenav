AC_INIT(xmlTreeNav,0.3.5pre)
AC_CONFIG_SRCDIR([configure.ac])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_INSTALL

dnl wxWindows -----
AM_OPTIONS_WXCONFIG
reqwx=3.0.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi
dnl --------------

dnl libxml
AM_PATH_XML2( 2.0.0, , AC_MSG_ERROR(libxml2 must be installed.) )
AC_CHECK_LIB(xslt, main, , AC_MSG_ERROR(libxslt must be installed.))
AC_CHECK_LIB(exslt, main, , AC_MSG_ERROR(libexslt must be installed.))

PKG_CHECK_MODULES(LIBXMLDIFF, libxmldiff >= 0.1,
		[],
		[AC_MSG_FAILURE([libxmldiff not found])]
		)


AC_ARG_ENABLE(debug,
[  --enable-debug    Turn on debugging],
[case "${enableval}" in
  yes) 
    DEBUGG="-g -H -fomit-frame-pointer -DDEBUG" 
    WXDEBUG="--debug"
    ;;
  no)  debug=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
esac],[debug=false])

dnl Internationalization
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])

dnl libxmldiff pkg-config
PKG_CHECK_MODULES([libxmldiff], [libxmldiff >= 0.2.0])

DEBUG="-g -H "
LIBS="$DEBUGG $LIBS $WX_LIBS $XML_LIBS $libxmldiff_LIBS"
CXXFLAGS="$DEBUGG $CXXFLAGS $WX_CXXFLAGS $XML_CXXFLAGS $libxmldiff_CXXFLAGS"
CPPFLAGS="$DEBUGG $CPPFLAGS $WX_CPPFLAGS $XML_CPPFLAGS $libxmldiff_CPPFLAGS"
CFLAGS="$DEBUGG $CFLAGS $WX_CFLAGS $XML_CFLAGS $libxmldiff_CFLAGS"

AC_CONFIG_FILES([ \
    Makefile \
    src/Makefile \
    lib/Makefile \
    lib/wxmisc/Makefile \
    po/Makefile.in \
    doc/Makefile \
    doc/use/Makefile \
    resources/Makefile \
    resources/xpm/Makefile \
    resources/treeicons/Makefile \
    ])
AC_OUTPUT
