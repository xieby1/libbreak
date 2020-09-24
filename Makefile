include common.mk
include config.mk

ARCH_DEP_SRC := arch/${ARCH}
CFLAGS_LIBBREAK = -Iinclude -Icapstone/include

all: test libbreak.a capstone

test: FORCE libbreak.a capstone
	${MAKE} -C test/

libbreak.a: ${ARCH_DEP_SRC}/break.o
	ar rcs $@ $<

${ARCH_DEP_SRC}/break.o: ${ARCH_DEP_SRC}/break.c include/break.h include/util.h
	${CC} -c ${CFLAGS} ${CFLAGS_LIBBREAK} -o $@ $<

capstone: FORCE
	CAPSTONE_STATIC=yes ${MAKE} -C capstone/

clean: FORCE clean-local
	${MAKE} clean -C capstone/

clean-local: FORCE
	rm -f libbreak.a
	rm -f ${ARCH_DEP_SRC}/*.o
	${MAKE} clean -C test/

FORCE:
