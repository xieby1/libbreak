CPU := $(shell uname -m)
ARCH_DEP_SRC := arch/mips/
CC := gcc
CFLAGS := -Iinclude/ -g
SRC := break

all: test

TEST_SRC := $(wildcard test/*.c)
TEST_PROGS := $(patsubst %.c, %, ${TEST_SRC})
test: ${TEST_PROGS}

test/%: test/%.c lib${SRC}.a
	${CC} $< ${CFLAGS} -L. -lbreak -o $@

lib${SRC}.a: ${ARCH_DEP_SRC}${SRC}.o
	ar rcs $@ $?

${ARCH_DEP_SRC}%.o: ${ARCH_DEP_SRC}%.c
	${CC} -c ${CFLAGS} -o $@ $<

clean:
	rm lib${SRC}.a
	rm ${ARCH_DEP_SRC}*.o
	rm ${TEST_PROGS}
