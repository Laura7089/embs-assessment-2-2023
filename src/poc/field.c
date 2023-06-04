#include <stdlib.h>
#include "field.h"

unsigned int idx(field* f, unsigned int x, unsigned int y) {
    // I'm just gonna let the segfault happen,
    // and correct logic bugs it throws up by hand
    //
    // Can always add a function to check if an index is valid
    /* if (x >= f->width || y >= f->height) { */
    /*     return f->num_tiles + 1; */
    /* } */

    // Column- vs row-major ordering doesn't matter here
    return f->inner[(x * f->height) + y];
}

tile* idxt(field* f, unsigned int x, unsigned int y) {
    return &(f->tiles[idx(f, x, y)]);
}

field new_field(unsigned int num_tiles, unsigned int x, unsigned int y) {
    field f;
    f.num_tiles = num_tiles;
    f.width = x;
    f.height = y;
    f.placed = calloc(num_tiles, sizeof(int));
    // TODO: is there a way to `malloc` these in the same place?
    f.tiles = malloc(sizeof(tile) * num_tiles);
    f.inner = malloc(sizeof(unsigned int) * x * y);

    for (int i = 0; i < (x * y); i++) {
        f.inner[i] = num_tiles;
    }

    return f;
}
