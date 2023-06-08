#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "field.h"

field new_field(unsigned int size) {
    field f;
    f.size = size;
    f.num_tiles = size * size;

    for (int i = 0; i < f.num_tiles; i++) {
        f.inner[i] = f.num_tiles;
        f.placed[i] = 0;
    }

    return f;
}

field fcopy(field* f) {
    return *f;
}

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

const colour INVALID = 127;

void sur_colours(field* f, colour* buf, coord cs) {
    for (int s = 0; s < 4; s++) {
        unsigned int i = idxo(f, cs, s);
        if (i == f->num_tiles) {
            // Location is out of bounds
            buf[s] = INVALID;
            continue;
        }

        if (i == f->num_tiles) {
            // No tile is placed there ergo anything will fit
            buf[s] = INVALID;
            continue;
        }

        buf[s] = get_side(f->tiles[i], opposite(s));
    }
}

int tile_fits(field* f, unsigned int t, coord cs) {
    // Figure out the sequence of colours we need
    colour needed_colours[4];
    sur_colours(f, needed_colours, cs);

    tile* ti = &f->tiles[t];
    // Try matching the colours to the file in 4 different rotations
    // TODO: there's room for optimisation here...
    for (int _r = 0; _r < 4; _r++) {
        int fits = 1;
        for (int s = 0; s < 4; s++) {
            if (needed_colours[s] == INVALID) {
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

unsigned int free_cells_dir(field* f, coord* buf, side d) {
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

            if (idxo(f, cs, d) != f->num_tiles) {
                // There's a placed tile adjacent!
                buf[bi] = cs;
                bi += 1;
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
    int buf_size = (f->num_tiles * 9) + (f->size * 3);
    int row_width = (f->size * 3) + 1;

    // Iterate top-down so the origin is in the bottom left
    int buf_offset = 0;
    for (int tile_y = f->size-1; tile_y >= 0; tile_y--) {
        // Top row
        for (int tile_xt = 0; tile_xt < f->size; tile_xt++) {
            int t = idx(f, c(tile_xt, tile_y));
            if (t != f->num_tiles) {
                sprintf(&buf[buf_offset], " %d ", get_side(f->tiles[t], top));
            } else {
                sprintf(&buf[buf_offset], "eee");
            }
            buf_offset += 3;
        }
        sprintf(&buf[buf_offset], "\n");
        buf_offset += 1;
        // Middle row
        for (int tile_xm = 0; tile_xm < f->size; tile_xm++) {
            int t = idx(f, c(tile_xm, tile_y));
            if (t != f->num_tiles) {
                tile ti = f->tiles[t];
                sprintf(&buf[buf_offset], "%d %d", get_side(ti, left), get_side(ti, right));
            } else {
                sprintf(&buf[buf_offset], "eee");
            }
            buf_offset += 3;
        }
        sprintf(&buf[buf_offset], "\n");
        buf_offset += 1;
        // Bottom row
        for (int tile_xb = 0; tile_xb < f->size; tile_xb++) {
            int t = idx(f, c(tile_xb, tile_y));
            if (t != f->num_tiles) {
                sprintf(&buf[buf_offset], " %d ", get_side(f->tiles[t], bottom));
            } else {
                sprintf(&buf[buf_offset], "eee");
            }
            buf_offset += 3;
        }

        if (tile_y != 0) {
            sprintf(&buf[buf_offset], "\n");
        }
        buf_offset += 1;
    }

    return buf_size;
}

void print_field(field* f) {
    char buf[(f->num_tiles * 9) + (f->size * 3)];
    repr_field(buf, f);
    printf("%s\n", buf);
}
