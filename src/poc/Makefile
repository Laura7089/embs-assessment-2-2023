CC = gcc
CFLAGS := -O3
DEBUG_CFLAGS := -O0 -g

SOURCES := main.c tile.c field.c solve.c
HEADERS := tile.h field.h solve.h

.PHONY: clean

main: ${SOURCES} ${HEADERS}
	${CC} ${CFLAGS} ${SOURCES} -o $@

main_debug: ${SOURCES} ${HEADERS}
	${CC} ${DEBUG_CFLAGS} ${SOURCES} -o $@

clean:
	rm -fv main main_debug
	rm -rfv known_solvable
