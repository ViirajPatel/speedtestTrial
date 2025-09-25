AC_INIT([lcm-speedtest],[0.1.0],[you@example.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])

# Checks for libraries.
AC_CHECK_LIB([m],[pow])

# Enable large file support
AC_SYS_LARGEFILE

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h sys/time.h sys/types.h sys/socket.h netdb.h arpa/inet.h fcntl.h errno.h])

# Configure optional features
AC_ARG_ENABLE([debug], AS_HELP_STRING([--enable-debug],[Enable debug logging]), [enable_debug=yes], [enable_debug=no])
if test "x$enable_debug" = "xyes"; then
  CFLAGS="$CFLAGS -O0 -g -DDEBUG"
else
  CFLAGS="$CFLAGS -O2"
fi

AC_CONFIG_FILES([Makefile src/Makefile include/Makefile])
AC_OUTPUT
