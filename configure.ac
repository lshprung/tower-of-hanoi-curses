#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([tower-of-hanoi-curses], [0.0.1])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AC_DEFINE([TITLE], ["Tower of Hanoi"], [Title to print at the top])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([memset])

# check for any curses library
AX_WITH_CURSES
if test "x$ax_cv_curses" != xyes; then
	AC_MSG_ERROR([requires an X/Open-compatible Curses library])
fi

# enables/disables
AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug], [enable debug output (disabled by default)])],
  [enable_debug=${enableval}],
  [enable_debug=no])
if test "x$enable_debug" = xyes; then
	AC_DEFINE_UNQUOTED([DEBUG], [1], [Enable debug printing])
else
	AC_DEFINE_UNQUOTED([DEBUG], [0], [Enable debug printing])
fi

# Setup Automake
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

AC_OUTPUT
