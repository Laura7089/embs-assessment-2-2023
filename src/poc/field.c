#include <stdlib.h>
#include <string.h>
#include "field.h"

// Get index into `f.inner` that `(x, y)` represent
//
// Not part of public API
unsigned int flatten(field* f, unsigned int x, unsigned int y) {
    return (x * f->size) + y;
}
// Get a pointer to a particular index of `inner`
//
// This is not part of public API
unsigned int* idxp(field* f, unsigned int x, unsigned int y) {
    // Column- vs row-major ordering doesn't matter here
    return &(f->inner[flatten(f, x, y)]);
}
unsigned int idx(field* f, unsigned int x, unsigned int y) {
    return *idxp(f, x, y);
}
tile* idxt(field* f, unsigned int x, unsigned int y) {
    return &(f->tiles[idx(f, x, y)]);
}
unsigned int idxo(field* f, unsigned int x, unsigned int y, side s) {
    switch (s) {
    case bottom:
        if (y == 0) {
            return f->num_tiles;
        }
        return f->inner[flatten(f, x, y-1)];
    case top:
        if (y >= f->size-1) {
            return f->num_tiles;
        }
        return f->inner[flatten(f, x, y+1)];
    case left:
        if (x == 0) {
            return f->num_tiles;
        }
        return f->inner[flatten(f, x-1, y)];
    case right:
        if (x == f->size-1) {
            return f->num_tiles;
        }
        return f->inner[flatten(f, x+1, y)];
    }
}

void place(field* f, unsigned int t, unsigned int x, unsigned int y) {
    if (f->placed[t]) {
        return;
    }

    *idxp(f, x, y) = t;
    f->placed[t] = 1;
}

void unplace(field* f, unsigned int x, unsigned int y) {
    unsigned int t = idx(f, x, y);
    if (t == f->num_tiles) {
        return;
    }

    *idxp(f, x, y) = f->num_tiles;
    f->placed[t] = 0;
}

field new_field(unsigned int size) {
    field f;
    f.size = size;
    f.num_tiles = size * size;
    f.placed = calloc(f.num_tiles, sizeof(int));
    // TODO: is there a way to `malloc` these in the same place?
    f.tiles = malloc(sizeof(tile) * f.num_tiles);
    f.inner = malloc(sizeof(unsigned int) * f.num_tiles);

    for (int i = 0; i < (f.num_tiles); i++) {
        f.inner[i] = f.num_tiles;
    }

    return f;
}

field copy(field* f) {
    field fnew;
    memcpy(&fnew, f, sizeof(field));

    fnew.tiles = malloc(sizeof(tile) * f->num_tiles);
    memcpy(fnew.tiles, f->tiles, sizeof(tile) * f->num_tiles);

    fnew.placed = malloc(sizeof(int) * f->num_tiles);
    memcpy(fnew.placed, f->placed, sizeof(int) * f->num_tiles);

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

int tile_fits(field* f, unsigned int t, unsigned int x, unsigned int y) {
    // Figure out the sequence of colours we need
    colour invalid = 127;
    colour needed_colours[4];
    for (int s = 0; s < 4; s++) {
        unsigned int i = idxo(f, x, y, s);
        if (i == f->num_tiles) {
            // Location is out of bounds
            needed_colours[s] = invalid;
            continue;
        }

        unsigned int adj = f->inner[i];
        if (adj == f->num_tiles) {
            // No tile is placed there ergo anything will fit
            needed_colours[s] = invalid;
            continue;
        }

        needed_colours[s] = get_side(f->tiles[adj], opposite(s));
    }

    tile ti = f->tiles[t];
    // Try matching the colours to the file in 4 different rotations
    // TODO: there's room for optimisation here...
    for (int _r = 0; _r < 4; _r++) {
        int fits = 1;
        for (int s = 0; s < 4; s++) {
            if (needed_colours[s] == invalid) {
                continue;
            } else if (get_side(ti, s) != needed_colours[s]) {
                fits = 0;
                break;
            }
        }

        if (fits) {
            return 1;
        }

        rotate_right(&ti);
    }

    return 0;
}

unsigned int num_unplaced(field* f) {
    unsigned int count = 0;
    for (int i = 0; i < f->num_tiles; i++) {
        if (f->placed[i]) {
            count += 1;
        }
    }

    return count;
}

int is_edge(field* f, unsigned int x, unsigned int y) {
    return (x == 0 || x == f->size || y == 0 || y == f->size);
}

void shift(field* f, side s) {
    if (touches_edge(f, s)) {
        return ;
    }

    switch (s) {
    case top:
        // Shunt up all rows
        for (int y = f->size-1; y > 0; y--) {
            for (int x = 0; x < f->size; x++) {
                *idxp(f, x, y) = idx(f, x, y-1);
            }
        }
        // "Zero out" the bottom row
        for (int x = 0; x < f->size; x++) {
            *idxp(f, x, 0) = f->num_tiles;
        }
        break;
    case bottom:
        // Shunt down all rows
        for (int y = 0; y < f->size-1; y++) {
            for (int x = 0; x < f->size; x++) {
                *idxp(f, x, y) = idx(f, x, y+1);
            }
        }
        // "Zero out" the top row
        for (int x = 0; x < f->size; x++) {
            *idxp(f, x, f->size-1) = f->num_tiles;
        }
        break;
    case right:
        // Shunt right all cols
        for (int x = f->size-1; x > 0; x--) {
            for (int y = 0; y < f->size; y++) {
                *idxp(f, x, y) = idx(f, x-1, y);
            }
        }
        // "Zero out" the left col
        for (int y = 0; y < f->size; y++) {
            *idxp(f, 0, y) = f->num_tiles;
        }
        break;
    case left:
        // Shunt left all cols
        for (int x = 0; x < f->size-1; x++) {
            for (int y = 0; y < f->size; y++) {
                *idxp(f, x, y) = idx(f, x+1, y);
            }
        }
        // "Zero out" the right col
        for (int y = 0; y < f->size; y++) {
            *idxp(f, f->size-1, y) = f->num_tiles;
        }
        break;
    }
}

int touches_edge(field* f, side s) {
    switch (s) {
    case top:
        for (int x = 0; x < f->size; x++) {
            if (idx(f, x, f->size - 1) != f->num_tiles) {
                return 1;
            }
        }
        break;
    case bottom:
        for (int x = 0; x < f->size; x++) {
            if (idx(f, x, 0) != f->num_tiles) {
                return 1;
            }
        }
        break;
    case right:
        for (int y = 0; y < f->size; y++) {
            if (idx(f, f->size - 1, y) != f->num_tiles) {
                return 1;
            }
        }
        break;
        break;
    case left:
        for (int y = 0; y < f->size; y++) {
            if (idx(f, 0, y) != f->num_tiles) {
                return 1;
            }
        }
        break;
    }

    return 0;
}
