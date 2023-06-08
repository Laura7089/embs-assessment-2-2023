CC = gcc
CFLAGS := -O3

SOURCES := main.c tile.c solve.c field.c
HEADERS := tile.h solve.h field.h

PUZZLES := puzzles

.PHONY: clean clean_all

main: ${SOURCES} ${HEADERS} ${PUZZLES}
	${CC} ${CFLAGS} ${SOURCES} -o $@

puzzles: generate_set.sh
	./generate_set.sh ${PUZZLES}
