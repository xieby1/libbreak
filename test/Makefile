include ../common.mk

SRC := $(wildcard *.c)
# OBJS := $(patsubst %.c, %.o, ${SRC})
PROGS := $(patsubst %.c, %, ${SRC})
# TODO: include libcapstone.a into libbreak.a,
# so that no need explicitly link against libcapstone.a
CFLAGS_LIBBREAK = -I../include -L.. -lbreak -L../capstone -lcapstone
CFLAGS_CAPSTONE = -I../capstone/include -L../capstone -lcapstone

all: ${PROGS}

break: break.c ../libbreak.a
	${CC} $< ${CFLAGS} ${CFLAGS_LIBBREAK} -o $@

capstone: capstone.c ../capstone/libcapstone.a
	${CC} $< ${CFLAGS} ${CFLAGS_CAPSTONE} -o $@

clean: FORCE
	rm -f ${PROGS}

FORCE:
