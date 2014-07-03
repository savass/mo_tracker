#Quiet compilation.
Q=@
CC = g++
xLIB = -std=gnu++0x
GLIB_CFLAGS  = -Wall $(shell pkg-config glib-2.0 --cflags) -fPIC

all: mo_tracker

clean: 
	$(Q)rm -f mo_tracker

mo_tracker: mo_tracker.cpp
	@echo OL $@
	$(Q)$(CC) $(xLIB) $(GLIB_CFLAGS) $(shell pkg-config libxml-2.0 gio-2.0 ivy-glib --cflags) -o  $@ $^ -lm -lz $(shell pkg-config libxml-2.0 gio-2.0 ivy-glib libpcre --libs)

