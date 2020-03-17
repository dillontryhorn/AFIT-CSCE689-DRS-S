#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/DivFinder.cpp])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netinet/in.h stdlib.h string.h strings.h sys/socket.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

AC_DEFINE([LARGEINT], [uint128_t], ["Unsigned int type to hold original value and calculations"])
AC_DEFINE([LARGEINT2X], [uint256_t], ["Unsigned int twice as large as LARGEINT (bit-wise)"])
AC_DEFINE([LARGESIGNED2X], [int512_t], ["Signed int made of twice the bits as LARGEINT2X"])

#AC_DEFINE([LARGEINT], [uint256_t], ["Unsigned int type to hold original value and calculations"])
#AC_DEFINE([LARGEINT2X], [uint512_t], ["Unsigned int twice as large as LARGEINT (bit-wise)"])
#AC_DEFINE([LARGESIGNED2X], [int1024_t], ["Signed int made of twice the bits as LARGEINT2X"])

# Checks for library functions.
AC_CHECK_FUNCS([bzero socket strtol select])
# Boost Libraries
#AC_CHECK_LIB([boost_multiprecision], [gcd], [], [
#   echo "You are missing the boost libraries. It is required for password authentication."
#   exit -1;
#   ])

AM_INIT_AUTOMAKE([subdir-objects -Wall])
AC_CONFIG_FILES([Makefile
		 src/Makefile])

AC_OUTPUT
