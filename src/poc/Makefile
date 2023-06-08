CC = gcc
CFLAGS := -O3

SOURCES := tile.c solve.c field.c
HEADERS := tile.h solve.h field.h

PUZZLES := puzzles

SIZE := 3
EPOCH := 0

.PHONY: clean clean_all main.c main_single.c

main: ${SOURCES} ${HEADERS} ${PUZZLES} main.c
	${CC} ${CFLAGS} ${SOURCES} main.c -o $@

main_single: ${SOURCES} ${HEADERS} ${PUZZLES} main_single.c
	${CC} ${CFLAGS} ${SOURCES} main_single.c -o $@

main.c:	main.c.j2
	jinja2 -D size=${SIZE} -D epoch=${EPOCH} -o $@ $^

main_single.c: main_single.c.j2
	jinja2 -D size=${SIZE} -D epoch=${EPOCH} -o $@ $^

puzzles: generate_set.sh
	./generate_set.sh ${PUZZLES}
