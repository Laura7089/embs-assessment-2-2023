CC = gcc
CFLAGS := -O3

SOURCES := main.c tile.c solve.c
HEADERS := tile.h solve.h

.PHONY: clean

main_malloc: ${SOURCES} ${HEADERS} known_solvable malloc/field.c malloc/field.h
	${CC} ${CFLAGS} ${SOURCES} -Imalloc malloc/field.c -o $@

main_heapless: ${SOURCES} ${HEADERS} known_solvable heapless/field.c heapless/field.h
	${CC} ${CFLAGS} ${SOURCES} -Iheapless heapless/field.c -o $@

known_solvable: generate_set.sh
	./generate_set.sh

clean:
	rm -fv main main_debug
	rm -rfv known_solvable
