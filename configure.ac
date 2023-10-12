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

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# check for any curses library
AX_WITH_CURSES
if test "x$ax_cv_curses" != xyes; then
	AC_MSG_ERROR([requires an X/Open-compatible Curses library])
fi

# Setup Automake
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

AC_OUTPUT
