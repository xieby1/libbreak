CPU := $(shell uname -m)
ARCH_DEP_SRC := arch/mips/
CC := gcc
CFLAGS := -Iinclude/
SRC := break

all: lib${SRC}.a

lib${SRC}.a: ${ARCH_DEP_SRC}${SRC}.o
	ar rcs $@ $?

${ARCH_DEP_SRC}%.o: ${ARCH_DEP_SRC}%.c
	${CC} -c ${CFLAGS} -o $@ $<

clean:
	rm lib${SRC}.a
	rm ${ARCH_DEP_SRC}*.o
