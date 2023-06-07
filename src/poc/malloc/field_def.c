#include <stdlib.h>
#include <string.h>
#include "field_def.h"
#include "../field.h"

field new_field(unsigned int size) {
    field f;
    f.size = size;
    f.num_tiles = size * size;
    f.placed = calloc(f.num_tiles, sizeof(int));
    // TODO: is there a way to `malloc` these in the same place?
    f.tiles = malloc(sizeof(tile) * f.num_tiles);
    f.inner = malloc(sizeof(unsigned int) * f.num_tiles);

    for (int i = 0; i < f.num_tiles; i++) {
        f.inner[i] = f.num_tiles;
    }

    return f;
}

field fcopy(field* f) {
    field fnew = *f;

    fnew.placed = malloc(sizeof(int) * f->num_tiles);
    memcpy(fnew.placed, f->placed, sizeof(int) * f->num_tiles);

    fnew.tiles = malloc(sizeof(tile) * f->num_tiles);
    memcpy(fnew.tiles, f->tiles, sizeof(tile) * f->num_tiles);

    fnew.inner = malloc(sizeof(unsigned int) * f->num_tiles);
    memcpy(fnew.inner, f->inner, sizeof(unsigned int) * f->num_tiles);

    return fnew;
}

void free_bufs(field* f) {
    free(f->tiles);
    f->tiles = NULL;
    free(f->inner);
    f->inner = NULL;
    free(f->placed);
    f->placed = NULL;
}
