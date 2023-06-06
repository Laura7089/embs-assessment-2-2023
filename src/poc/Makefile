CC = gcc
CFLAGS := -O3
DEBUG_CFLAGS := -O0 -g

SOURCES := main.c tile.c field.c solve.c

.PHONY: clean

main: ${SOURCES}
	${CC} ${CFLAGS} $^ -o $@

main_debug: ${SOURCES}
	${CC} ${DEBUG_CFLAGS} $^ -o $@

clean:
	rm -fv main main_debug
	rm -rfv known_solvable
