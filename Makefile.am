ACLOCAL_AMFLAGS = -I m4

srcdir = @srcdir@
VPATH = @srcdir@

bin_PROGRAMS = gautier_scanner

src/barcode_record.cpp: Makefile 
src/scan_config_record.cpp: Makefile 
src/scan_config_window.cpp: Makefile 
src/main_window.cpp: Makefile 
src/application.cpp: Makefile 
src/app.cpp: Makefile 

gautier_scanner_CXXFLAGS = $(GTKMM_CFLAGS) 
gautier_scanner_LDADD = $(GTKMM_LIBS) 
gautier_scanner_SOURCES = src/barcode_record.cpp \
src/scan_config_record.cpp \
src/scan_config_window.cpp \
src/main_window.cpp \
src/application.cpp \
src/app.cpp 

gautier_scanner_CPPFLAGS = -I$(top_srcdir)/src


