#						-*- Autoconf -*-
# Process this file with autotools to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libcmap, 0.1)
AM_INIT_AUTOMAKE
LT_INIT
AC_CONFIG_SRCDIR([map.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h:config.h.in])

# Check for the compilation tools
AC_LANG(C)
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AC_PROG_RANLIB
AC_PATH_PROG([ATFRUN], [atf-run])
AC_PATH_PROG([ATFREPORT], [atf-report])

# Check for headers and types
AC_HEADER_STDC
AC_HEADER_TIME
AC_HEADER_STDBOOL

AC_TYPE_SIZE_T

AC_ARG_WITH(arpc,[AS_HELP_STRING([--with-arpc], [Use Asynchronous RPC library])],
	[case "$withval" in
	  yes)
		ac_with_arpc=yes
		;;
	  no)
		ac_with_arpc=no
		;;
	  *)
		ac_with_arpc=yes
		ARPC_INCLUDES="-I$withval"
		ARPC_LDFLAGS="-L$withval/libarpc"
		;;
	esac],
	[ac_with_arpc=check]
)
AM_CONDITIONAL(ARPC, test x$ac_with_arpc = xyes)
AC_SUBST(ARPC_INCLUDES)
AC_SUBST(ARPC_LDFLAGS)

AC_ARG_WITH(atf,[AS_HELP_STRING([--with-atf], [Use NetBSD ATF])],
	[case "$withval" in
	  yes)
		ac_with_atf=yes
		;;
	  no)
		ac_with_atf=no
		;;
	  *)
		ac_with_atf=yes
		ATF_INCLUDES="-I$withval/include"
		ATF_LDFLAGS="-L$withval/lib"
		;;
	esac],
	[ac_with_atf=yes]
)
AM_CONDITIONAL(ATF, test x$ac_with_atf = xyes)
AC_SUBST(ATF_INCLUDES)
AC_SUBST(ATF_LDFLAGS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
