CC = gcc
CFLAGS := -O0 -g

.PHONY: clean

default: main.c tile.c field.c
	${CC} ${CFLAGS} main.c tile.c field.c -o main

clean:
	rm -fv main
