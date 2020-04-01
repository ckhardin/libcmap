#						-*- Autoconf -*-
# Process this file with autotools to produce a configure script.

AC_INIT([libcmap], [0.1], [], [libcmap],
        [https://github.com/ckhardin/libcmap])
AC_PREREQ(2.65)

AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])
LT_INIT
AC_CONFIG_SRCDIR([map.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h:config.h.in])

# Check for the compilation tools
AC_LANG(C)
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CC_STDC
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Check for headers and types
AC_HEADER_STDC
AC_HEADER_TIME
AC_HEADER_STDBOOL

AC_TYPE_SIZE_T

AC_ARG_WITH(arpc,[AS_HELP_STRING([--with-arpc],
	[Use Asynchronous RPC library])],
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

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
