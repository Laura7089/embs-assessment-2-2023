UNITY := unity/src
SOURCES := ../field.c ../tile.c ../solve.c

CC := gcc
CFLAGS := -I${UNITY} -O0 -g

TESTS := field_test

METHOD := heapless

.PHONY: clean

test_%: %.c test_%.c ${SOURCES} ${UNITY}/unity.c ../${METHOD}/field_def.c
	${CC} ${CFLAGS} -I../${METHOD} $^ -o $@

test_%.c: %.c
	./unity/auto/generate_test_runner.rb $^ $@

clean:
	rm -fv run_*
