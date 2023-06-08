CC = gcc
CFLAGS := -O3

SOURCES := main.c tile.c solve.c field.c
HEADERS := tile.h solve.h field.h

PUZZLES := puzzles

SIZE := 3
EPOCH := 0

.PHONY: clean clean_all

main: ${SOURCES} ${HEADERS} ${PUZZLES}
	${CC} ${CFLAGS} ${SOURCES} -o $@

main.c:	main.c.j2
	jinja2 -D size=${SIZE} -D epoch=${EPOCH} -o $@ $^

puzzles: generate_set.sh
	./generate_set.sh ${PUZZLES}
