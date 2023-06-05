#include <stdlib.h>
#include <string.h>
#include "field.h"

// Get a pointer to a particular index of `inner`
//
// This is not part of public API
unsigned int* idxp(field* f, unsigned int x, unsigned int y) {
    // Column- vs row-major ordering doesn't matter here
    return &(f->inner[(x * f->num_tiles) + y]);
}
unsigned int idx(field* f, unsigned int x, unsigned int y) {
    return *idxp(f, x, y);
}
tile* idxt(field* f, unsigned int x, unsigned int y) {
    return &(f->tiles[idx(f, x, y)]);
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

field new_field(unsigned int num_tiles) {
    field f;
    f.num_tiles = num_tiles;
    f.placed = calloc(num_tiles, sizeof(int));
    // TODO: is there a way to `malloc` these in the same place?
    f.tiles = malloc(sizeof(tile) * num_tiles);
    f.inner = malloc(sizeof(unsigned int) * num_tiles * num_tiles);

    for (int i = 0; i < (num_tiles * num_tiles); i++) {
        f.inner[i] = num_tiles;
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

    fnew.inner = malloc(sizeof(unsigned int) * f->num_tiles * f->num_tiles);
    memcpy(fnew.inner, f->inner, sizeof(unsigned int) * f->num_tiles * f->num_tiles);

    return fnew;
}

void free_bufs(field* f) {
    free(f->tiles);
    free(f->inner);
    free(f->placed);
}

int tile_fits(field* f, unsigned int t, unsigned int x, unsigned int y) {
    // TODO
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
    return (x == 0 || x == f->num_tiles || y == 0 || y == f->num_tiles);
}

void shift(field* f, side s) {
    if (touches_edge(f, s)) {
        return ;
    }

    switch (s) {
    case top:
        // Shunt up all rows
        for (int y = f->num_tiles-1; y > 0; y--) {
            for (int x = 0; x < f->num_tiles; x++) {
                *idxp(f, x, y) = idx(f, x, y-1);
            }
        }
        // "Zero out" the bottom row
        for (int x = 0; x < f->num_tiles; x++) {
            *idxp(f, x, 0) = f->num_tiles;
        }
        break;
    case bottom:
        // Shunt down all rows
        for (int y = 0; y < f->num_tiles-1; y++) {
            for (int x = 0; x < f->num_tiles; x++) {
                *idxp(f, x, y) = idx(f, x, y+1);
            }
        }
        // "Zero out" the top row
        for (int x = 0; x < f->num_tiles; x++) {
            *idxp(f, x, f->num_tiles-1) = f->num_tiles;
        }
        break;
    case right:
        // Shunt right all cols
        for (int x = f->num_tiles-1; x > 0; x--) {
            for (int y = 0; y < f->num_tiles; y++) {
                *idxp(f, x, y) = idx(f, x-1, y);
            }
        }
        // "Zero out" the left col
        for (int y = 0; y < f->num_tiles; y++) {
            *idxp(f, 0, y) = f->num_tiles;
        }
        break;
    case left:
        // Shunt left all cols
        for (int x = 0; x < f->num_tiles-1; x++) {
            for (int y = 0; y < f->num_tiles; y++) {
                *idxp(f, x, y) = idx(f, x+1, y);
            }
        }
        // "Zero out" the right col
        for (int y = 0; y < f->num_tiles; y++) {
            *idxp(f, f->num_tiles-1, y) = f->num_tiles;
        }
        break;
    }
}

int touches_edge(field* f, side s) {
    switch (s) {
    case top:
        for (int x = 0; x < f->num_tiles; x++) {
            if (idx(f, x, f->num_tiles - 1) != f->num_tiles) {
                return 1;
            }
        }
        break;
    case bottom:
        for (int x = 0; x < f->num_tiles; x++) {
            if (idx(f, x, 0) != f->num_tiles) {
                return 1;
            }
        }
        break;
    case right:
        for (int y = 0; y < f->num_tiles; y++) {
            if (idx(f, f->num_tiles - 1, y) != f->num_tiles) {
                return 1;
            }
        }
        break;
        break;
    case left:
        for (int y = 0; y < f->num_tiles; y++) {
            if (idx(f, 0, y) != f->num_tiles) {
                return 1;
            }
        }
        break;
    }

    return 0;
}
