bin_PROGRAMS = speedtestd

speedtestd_SOURCES = main.c speedtest.c http.c util.c

# Add top-level include directory
AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = -Wall -Wextra -Werror -std=c11
speedtestd_LDADD = -lpthread -lm
