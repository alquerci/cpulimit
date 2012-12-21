#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cpulimit], [2.1.0-Beta], [alquerci@email.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/Config.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([malloc.h stdlib.h string.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([atexit setlocale strchr strspn])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
