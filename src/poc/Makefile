CC = gcc
CFLAGS := -O3

SOURCES := main.c tile.c solve.c field.c
HEADERS := tile.h solve.h field.h

.PHONY: clean

main_malloc: ${SOURCES} ${HEADERS} known_solvable malloc/field_def.c malloc/field_def.h
	${CC} ${CFLAGS} -Imalloc ${SOURCES} malloc/field_def.c -o $@

main_heapless: ${SOURCES} ${HEADERS} known_solvable heapless/field_def.c heapless/field_def.h
	${CC} ${CFLAGS} -Iheapless ${SOURCES} heapless/field_def.c -o $@

known_solvable: generate_set.sh
	./generate_set.sh

clean:
	rm -fv main main_debug
	rm -rfv known_solvable
