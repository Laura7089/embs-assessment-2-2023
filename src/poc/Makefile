CC = gcc
CFLAGS := -O3
DEBUG_CFLAGS := -O0 -g

.PHONY: clean

main: main.c tile.c field.c
	${CC} ${CFLAGS} main.c tile.c field.c -o $@

main_debug: main.c tile.c field.c
	${CC} ${DEBUG_CFLAGS} main.c tile.c field.c -o $@

clean:
	rm -fv main main_debug
	rm -rfv known_solvable
