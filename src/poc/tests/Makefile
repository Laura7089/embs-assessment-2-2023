UNITY := unity/src
SOURCES := ../field.c ../tile.c

CC := gcc
CFLAGS := -I${UNITY} -O0 -g

.PHONY: clean default

run_%: %.c run_%.c ${SOURCES} ${UNITY}/unity.c
	${CC} ${CFLAGS} $^ -o $@

run_%.c: %.c
	./unity/auto/generate_test_runner.rb $^ $@

clean:
	rm -fv ${BIN}
	rm -fv run_*
