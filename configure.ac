#						-*- Autoconf -*-
# Process this file with autotools to produce a configure script.

AC_INIT([libcmap], [0.1], [], [libcmap],
        [https://github.com/ckhardin/libcmap])
AC_PREREQ(2.65)

AC_CONFIG_SRCDIR([map.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h:config.h.in])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT

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

# Check for compile flags
AC_MSG_CHECKING(whether compiler understands -Wall)
old_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Wall"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],
	AC_MSG_RESULT(yes),
	AC_MSG_RESULT(no)
	CFLAGS="$old_CFLAGS"
)
AC_MSG_CHECKING(whether compiler understands -Werror)
old_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Werror"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],
	AC_MSG_RESULT(yes),
	AC_MSG_RESULT(no)
	CFLAGS="$old_CFLAGS"
)

# Check for test-anything-support
AC_REQUIRE_AUX_FILE([tap-driver.sh])
AC_PROG_AWK

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
