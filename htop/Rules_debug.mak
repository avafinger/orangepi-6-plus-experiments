# Rules.mak for executables

CC            = gcc
MAKE          = make

BIN_CFLAGS    = -Wall -g -O0 -DLinux -DDEBUG -D_XOPEN_SOURCE_EXTENDED -DSYSCONFDIR=\"\" -I../include
BIN_LDFLAGS   = -ldl 
BIN_LDFLAGS_T = -lz -lssl -lcrypto -lresolv -lpthread -ldl -lm -lncursesw -lutil
BIN_TARGETDIR = ../distrib/bin

LIB_CFLAGS    = -Wall -g -O0 -fPIC -I../../../include
LIB_LDFLAGS   = -shared
LIB_TARGETDIR = ../../../distrib/lib

MOD_CFLAGS    = -Wall -g -O0 -fPIC -I../../../include
MOD_LDFLAGS   = -shared
MOD_TARGETDIR = ../../../distrib/lib

TOP_BUILD_DIR = ${PWD}
INCLUDE_DIR = ${PWD}

.c.o:
	@echo "  DEBUG: CC $@" ; $(CC) $(CFLAGS) -c $< -o $@
