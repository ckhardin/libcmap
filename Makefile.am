ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = $(ARPC_INCLUDES)
AM_LDFLAGS = $(ARPC_LDFLAGS)

lib_LTLIBRARIES = libcmap.la

libcmap_ladir = $(includedir)/libcmap
libcmap_la_HEADERS = map.h
libcmap_la_SOURCES = map.c map_types.c

if ARPC

libcmap_la_SOURCES += map_axdr.c

endif

include tests/Makefile.am.inc
