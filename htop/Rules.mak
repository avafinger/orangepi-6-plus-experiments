# Rules.mak for executables

CC            = gcc
MAKE          = make -s

BIN_CFLAGS    = -Wall -O2 -DDEBUG -D_XOPEN_SOURCE_EXTENDED -DSYSCONFDIR=\"\" -I../include
BIN_LDFLAGS   = -s -ldl 
BIN_LDFLAGS_T = -s -lz -lssl -lcrypto -lresolv -lpthread -ldl -lm -lncursesw -lutil
BIN_TARGETDIR = ../distrib/bin

LIB_CFLAGS    = -Wall -O2 -fPIC -I../../../include
LIB_LDFLAGS   = -s -shared
LIB_TARGETDIR = ../../../distrib/lib

MOD_CFLAGS    = -Wall -O2 -fPIC -I../../../include
MOD_LDFLAGS   = -s -shared
MOD_TARGETDIR = ../../../distrib/lib

TOP_BUILD_DIR = ${PWD}
INCLUDE_DIR = ${PWD}

.c.o:
	@echo "  RELEASE CC $@" ; $(CC) $(CFLAGS) -c $< -o $@
