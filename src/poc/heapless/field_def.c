#include <stdlib.h>
#include <string.h>
#include "field_def.h"
#include "../field.h"

field new_field(unsigned int size, tile* tiles) {
    field f;
    f.size = size;
    f.num_tiles = size * size;

    for (int i = 0; i < f.num_tiles; i++) {
        f.inner[i] = f.num_tiles;
        f.placed[i] = 0;
        f.tiles[i] = tiles[i];
    }

    return f;
}

field fcopy(field* f) {
    return *f;
}

void free_bufs(field* f) {}
