include ../common.mk

SRC := $(wildcard *.c)
# OBJS := $(patsubst %.c, %.o, ${SRC})
PROGS := $(patsubst %.c, %, ${SRC})
CFLAGS_LIBBREAK = -I../include -L.. -lbreak
CFLAGS_CAPSTONE = -I../capstone/include -L../capstone -lcapstone

all: ${PROGS}

break: break.c ../libbreak.a
	${CC} $< ${CFLAGS} ${CFLAGS_LIBBREAK} -o $@

capstone: capstone.c ../capstone/libcapstone.a
	${CC} $< ${CFLAGS} ${CFLAGS_CAPSTONE} -o $@

clean: FORCE
	rm -f ${PROGS}

FORCE:
