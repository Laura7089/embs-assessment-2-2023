UNITY := unity/src
SOURCES := ../field.c ../tile.c ../solve.c

CC := gcc
CFLAGS := -I${UNITY} -O0 -g

TESTS := field_test

.PHONY: clean

run_%: %.c run_%.c ${SOURCES} ${UNITY}/unity.c ../malloc/field_def.c
	${CC} ${CFLAGS} -I../malloc $^ -o $@

run_%.c: %.c
	./unity/auto/generate_test_runner.rb $^ $@

run_%_heapless: %.c run_%.c ${SOURCES} ${UNITY}/unity.c ../heapless/field_def.c
	${CC} ${CFLAGS} -I ../heapless $^ -o $@

clean:
	rm -fv run_*
