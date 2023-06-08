#include <stdlib.h>
#include "solve.h"
#include "field.h"
#include "tile.h"

void overwrite(field* l, field* r) {
    free_bufs(l);
    *l = *r;
}

int solve(field* f) {
    coord* frees = malloc((f->num_tiles-1) * sizeof(coord));
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
            if (solve(&fs)) {
                overwrite(f, &fs);
                return 1;
            }
        }
        free_bufs(&fs);
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
        coord* frees = malloc(num_unplaced(&fs) * sizeof(coord));
        unsigned int num_free = free_cells(&fs, frees);
        for (int i = 0; i < num_free; i++) {
            // Make another copy
            field fsp = fcopy(&fs);

            // Try to place something
            if (try_place_any(&fsp, frees[i])) {
                if (solve(&fsp)) {
                    overwrite(f, &fsp);
                    return 1;
                }
            }
            free_bufs(&fsp);
        }
        free_bufs(&fs);
    }

    return 0;
}
