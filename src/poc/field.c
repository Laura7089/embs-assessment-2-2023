#include <stdlib.h>
#include <string.h>
#include "field.h"

// Get index into `f.inner` that `(cs.x, cs.y)` represent
//
// Not part of public API
unsigned int flatten(field* f, unsigned int x, unsigned int y) {
    return (x * f->size) + y;
}
// Get a pointer to a particular index of `inner`
//
// This is not part of public API
unsigned int* idxp(field* f, coord cs) {
    // Column- vs row-major ordering doesn't matter here
    return &(f->inner[flatten(f, cs.x, cs.y)]);
}
unsigned int idx(field* f, coord cs) {
    return *idxp(f, cs);
}
tile* idxt(field* f, coord cs) {
    return &(f->tiles[idx(f, cs)]);
}
unsigned int idxo(field* f, coord cs, side s) {
    switch (s) {
    case bottom:
        if (cs.y == 0) {
            return f->num_tiles;
        }
        return f->inner[flatten(f, cs.x, cs.y-1)];
    case top:
        if (cs.y >= f->size-1) {
            return f->num_tiles;
        }
        return f->inner[flatten(f, cs.x, cs.y+1)];
    case left:
        if (cs.x == 0) {
            return f->num_tiles;
        }
        return f->inner[flatten(f, cs.x-1, cs.y)];
    case right:
        if (cs.x == f->size-1) {
            return f->num_tiles;
        }
        return f->inner[flatten(f, cs.x+1, cs.y)];
    }
}

void place(field* f, unsigned int t, coord cs) {
    if (f->placed[t]) {
        return;
    }

    *idxp(f, cs) = t;
    f->placed[t] = 1;
}

void unplace(field* f, coord cs) {
    unsigned int t = idx(f, cs);
    if (t == f->num_tiles) {
        return;
    }

    *idxp(f, cs) = f->num_tiles;
    f->placed[t] = 0;
}

int tile_fits(field* f, unsigned int t, coord cs) {
    // Figure out the sequence of colours we need
    colour invalid = 127;
    colour needed_colours[4];
    for (int s = 0; s < 4; s++) {
        unsigned int i = idxo(f, cs, s);
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

    tile* ti = &f->tiles[t];
    // Try matching the colours to the file in 4 different rotations
    // TODO: there's room for optimisation here...
    for (int _r = 0; _r < 4; _r++) {
        int fits = 1;
        for (int s = 0; s < 4; s++) {
            if (needed_colours[s] == invalid) {
                continue;
            } else if (get_side(*ti, s) != needed_colours[s]) {
                fits = 0;
                break;
            }
        }

        if (fits) {
            return 1;
        }

        rotate_right(ti);
    }

    return 0;
}

unsigned int num_unplaced(field* f) {
    unsigned int count = 0;
    for (int i = 0; i < f->num_tiles; i++) {
        if (!f->placed[i]) {
            count += 1;
        }
    }

    return count;
}

int is_edge(field* f, coord cs) {
    return (cs.x == 0 || cs.x == f->size || cs.y == 0 || cs.y == f->size);
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
                *idxp(f, c(x, y)) = idx(f, c(x, y-1));
            }
        }
        // "Zero out" the bottom row
        for (int x = 0; x < f->size; x++) {
            *idxp(f, c(x, 0)) = f->num_tiles;
        }
        break;
    case bottom:
        // Shunt down all rows
        for (int y = 0; y < f->size-1; y++) {
            for (int x = 0; x < f->size; x++) {
                *idxp(f, c(x, y)) = idx(f, c(x, y+1));
            }
        }
        // "Zero out" the top row
        for (int x = 0; x < f->size; x++) {
            *idxp(f, c(x, f->size-1)) = f->num_tiles;
        }
        break;
    case right:
        // Shunt right all cols
        for (int x = f->size-1; x > 0; x--) {
            for (int y = 0; y < f->size; y++) {
                *idxp(f, c(x, y)) = idx(f, c(x-1, y));
            }
        }
        // "Zero out" the left col
        for (int y = 0; y < f->size; y++) {
            *idxp(f, c(0, y)) = f->num_tiles;
        }
        break;
    case left:
        // Shunt left all cols
        for (int x = 0; x < f->size-1; x++) {
            for (int y = 0; y < f->size; y++) {
                *idxp(f, c(x, y)) = idx(f, c(x+1, y));
            }
        }
        // "Zero out" the right col
        for (int y = 0; y < f->size; y++) {
            *idxp(f, c(f->size-1, y)) = f->num_tiles;
        }
        break;
    }
}

int touches_edge(field* f, side s) {
    switch (s) {
    case top:
        for (int x = 0; x < f->size; x++) {
            if (idx(f, c(x, f->size-1)) != f->num_tiles) {
                return 1;
            }
        }
        break;
    case bottom:
        for (int x = 0; x < f->size; x++) {
            if (idx(f, c(x, 0)) != f->num_tiles) {
                return 1;
            }
        }
        break;
    case right:
        for (int y = 0; y < f->size; y++) {
            if (idx(f, c(f->size-1, y)) != f->num_tiles) {
                return 1;
            }
        }
        break;
    case left:
        for (int y = 0; y < f->size; y++) {
            if (idx(f, c(0, y)) != f->num_tiles) {
                return 1;
            }
        }
        break;
    }

    return 0;
}

int is_full(field* f) {
    return (num_unplaced(f) == 0);
}

unsigned int free_cells(field* f, coord* buf) {
    if (is_full(f)) {
        return 0;
    }

    unsigned int bi = 0;

    for (int x = 0; x < f->size; x++) {
        for (int y = 0; y < f->size; y++) {
            coord cs = c(x, y);
            if (idx(f, cs) != f->num_tiles) {
                // There's already a tile here
                continue;
            }

            for (int s = 0; s < 4; s++) {
                if (idxo(f, cs, s) != f->num_tiles) {
                    // There's a placed tile adjacent!
                    buf[bi] = cs;
                    bi += 1;
                    break;
                }
            }
        }
    }

    return bi;
}

int try_place(field* f, unsigned int t, coord cs) {
    if (f->placed[t] || idx(f, cs) != f->num_tiles) {
        return 0;
    }

    if (tile_fits(f, t, cs)) {
        place(f, t, cs);
        return 1;
    } else {
        return 0;
    }
}

int try_place_any(field* f, coord cs) {
    for (int t = 0; t < f->num_tiles; t++) {
        if (try_place(f, t, cs)) {
            return 1;
        }
    }

    return 0;
}

const char EMPTY[12] = "eee\neee\neee";
unsigned int repr_field(char* buf, field* f) {
    char tile_reprs[f->num_tiles][12];

    // First get representations of all the tiles
    for (int x = 0; x < f->size; x++) {
        for (int y = 0; y < f->size; y++) {
            unsigned int t = idx(f, c(x, y));
            // Index into tile_reprs that we want
            unsigned int tri = flatten(f, x, y);
            if (t == f->num_tiles) {
                // If there's no tile there
                memcpy(&tile_reprs[tri][0], &EMPTY, 12 * sizeof(char));
                continue;
            }
            repr_tile(&tile_reprs[tri][0], f->tiles[t]);
        }
    }

    unsigned int bytes_written = 0;
    // Width (which we want) of one side of tile repr
    int TR_WIDTH = 3;
    size_t BYTES_PER_LOOP = sizeof(char) * TR_WIDTH;

    int num_columns = f->size * TR_WIDTH;
    int row_width = num_columns + 1;

    for (int y = 0; y < num_columns; y++) {
        int start_of_row = y * row_width;
        // Row offset into tile repr
        // We add one to skip the newlines
        int tri = (y % TR_WIDTH) * (TR_WIDTH + 1);

        int tile_y = y / TR_WIDTH;
        for (int tile_x = 0; tile_x < f->size; tile_x++) {
            char* this_tile = &tile_reprs[flatten(f, tile_x, tile_y)][0];
            // Start index into `buf`
            int bi = start_of_row + (tile_x * TR_WIDTH);

            // Copy characters into `buf`
            memcpy(&buf[bi], &this_tile[tri], BYTES_PER_LOOP);
            bytes_written += BYTES_PER_LOOP;
        }

        if (y != num_columns-1) {
            buf[start_of_row + row_width - 1] = '\n';
        } else {
            buf[start_of_row + row_width - 1] = '\0';
        }
        bytes_written += sizeof(char);
    }

    return bytes_written;
}
