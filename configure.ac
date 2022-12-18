AC_INIT([xmlTreeNav],[0.3.4-2])
AC_CONFIG_SRCDIR([configure.ac])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_INSTALL


dnl wxWindows -----
WX_CONFIG_OPTIONS
WX_STANDARD_OPTIONS([debug,unicode,shared,toolkit,wxshared])

WX_DEBUG=$DEBUG
WX_UNICODE=$UNICODE

WX_CONVERT_STANDARD_OPTIONS_TO_WXCONFIG_FLAGS
WX_CONFIG_CHECK([3.0.0], [wxWin=1],,[html,core,net,base],[$WXCONFIG_FLAGS])
WX_DETECT_STANDARD_OPTION_VALUES

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
AM_GNU_GETTEXT_VERSION([0.21])


dnl libxml
#AM_PATH_XML2( 2.0.0, , AC_MSG_ERROR(libxml2 must be installed.) )
#AC_CHECK_LIB(xslt, main, , AC_MSG_ERROR(libxslt must be installed.))
#AC_CHECK_LIB(exslt, main, , AC_MSG_ERROR(libexslt must be installed.))

PKG_CHECK_MODULES([libxml], [libxml-2.0 >= 2.6.26 libxslt libexslt])

dnl libxmldiff pkg-config
PKG_CHECK_MODULES([libxmldiff], [libxmldiff >= 0.2.0])

# Static link attempts: -static -static-libgcc -static-libstdc++

DEBUG="-g -H "
LIBS="$DEBUGG $LIBS $WX_LIBS $libxml_LIBS $libxmldiff_LIBS "
CXXFLAGS="$DEBUGG $CXXFLAGS $WX_CXXFLAGS $libxml_CFLAGS $libxmldiff_CFLAGS "
CPPFLAGS="$DEBUGG $CPPFLAGS $WX_CPPFLAGS $libxml_CFLAGS $libxmldiff_CFLAGS "
CFLAGS="$DEBUGG $CFLAGS $WX_CFLAGS $libxml_CFLAGS $libxmldiff_CFLAGS "

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

WX_STANDARD_OPTIONS_SUMMARY_MSG
