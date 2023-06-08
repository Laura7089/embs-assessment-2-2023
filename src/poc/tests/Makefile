UNITY := unity/src
SOURCES := ../field.c ../tile.c ../solve.c

CC := gcc
CFLAGS := -I${UNITY} -O0 -g

.PHONY: clean

test_%: %.c test_%.c ${SOURCES} ${UNITY}/unity.c
	${CC} ${CFLAGS} $^ -o $@

test_%.c: %.c
	./unity/auto/generate_test_runner.rb $^ $@
