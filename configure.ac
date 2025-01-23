AC_PREREQ([2.69])
AC_INIT([gautier_scanner], [0.0.1], [mklinfinity@proton.me])
AM_INIT_AUTOMAKE([gnits] [subdir-objects] [-Wall -Werror])
AM_PROG_AR
LT_PREREQ([2.4])
LT_INIT([dlopen])
AC_LANG([C++])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([src/app.cpp])
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL

# Check for a C++ compiler
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_INSTALL

# Check for pkg-config
PKG_PROG_PKG_CONFIG

# Check for GTKMM 4.0
PKG_CHECK_MODULES([GTKMM], [gtkmm-4.0 >= 4.0.0], ,
  AC_MSG_ERROR([GTKMM 4.0 or later is required but not found.])
)

# Output variables for Makefile
AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)

# Create config header (if needed)
AC_CONFIG_HEADERS([config.h])

# Generate Makefiles
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
