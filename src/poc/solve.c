#include <stdlib.h>
#include "solve.h"
#include "field.h"
#include "tile.h"

void overwrite(field* l, field* r) {
    *l = *r;
}

int solve_inner(field* f) {
    coord frees[f->num_tiles / 2];
    unsigned int num_free = free_cells(f, frees);

    // Base case
    if (num_free == 1) {
        if (try_place_any(f, frees[0])) {
            return 1;
        } else {
            return 0;
        }
    }

    // Recursive case(s)
    // Case 1: plain ol' placing
    for (int i = 0; i < num_free; i++) {
        field fs = fcopy(f);
        if (try_place_any(&fs, frees[i])) {
            if (solve_inner(&fs)) {
                overwrite(f, &fs);
                return 1;
            }
        }
    }

    // Case 2: start shiftin'
    for (int d = 0; d < 4; d++) {
        if (touches_edge(f, d)) {
            // We can't shift in this direction
            continue;
        }

        // Make a copy and shift it in the given direction
        field fs = fcopy(f);
        shift(&fs, d);

        // Try to place a tile
        // We have to try to place one otherwise the `solve` calls will keep shifting back and forth
        coord frees[fs.num_tiles / 2];
        unsigned int num_free = free_cells(&fs, frees);
        for (int i = 0; i < num_free; i++) {
            // Make another copy
            field fsp = fcopy(&fs);

            // Try to place something
            if (try_place_any(&fsp, frees[i])) {
                if (solve_inner(&fsp)) {
                    overwrite(f, &fsp);
                    return 1;
                }
            }
        }
    }

    return 0;
}

int solve(field* solved, field* f) {
    field fi = fcopy(f);

    place(&fi, 0, c(0, 0));
    if (solve_inner(&fi)) {
        solved[0] = fi;
        return 1;
    } else {
        return 0;
    }
}
