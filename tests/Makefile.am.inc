TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
		  $(top_srcdir)/build-aux/tap-driver.sh

TESTS =

TESTS += test_cmap
check_PROGRAMS = test_cmap
test_cmap_SOURCES = tests/test_cmap.c
