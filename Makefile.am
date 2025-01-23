ACLOCAL_AMFLAGS = -I m4

srcdir = @srcdir@
VPATH = @srcdir@

bin_PROGRAMS = gautier_scanner

src/app.cpp: Makefile
src/main_window.cpp: Makefile
src/barcode_record.cpp: Makefile

gautier_scanner_CXXFLAGS = $(GTKMM_CFLAGS)
gautier_scanner_LDADD = $(GTKMM_LIBS)
gautier_scanner_SOURCES = src/app.cpp \
src/main_window.cpp \
src/barcode_record.cpp 

gautier_scanner_CPPFLAGS = -I$(top_srcdir)/src


