#include <stdlib.h>
#include "solve.h"
#include "field.h"
#include "tile.h"

void solve_inner(field* f, field* solved, int* num_solved) {
    if (*num_solved == MAX_SOLVED) {
        return;
    }

    coord frees[f->num_tiles / 2];
    unsigned int num_free = free_cells(f, frees);

    // Base case
    if (num_free == 1) {
        solved[*num_solved] = fcopy(f);
        if (try_place_any(&solved[*num_solved], frees[0])) {
            *num_solved += 1;
        }
        // Return immediately because there's no point recursing here
        return;
    }

    // Recursive case(s)
    // Case 1: plain ol' placing
    for (int i = 0; i < num_free; i++) {
        field fs = fcopy(f);
        if (try_place_any(&fs, frees[i])) {
            solve_inner(&fs, solved, num_solved);
            if (*num_solved == MAX_SOLVED) {
                return;
            }
        }
    }

    // Case 2: start shiftin'
    //
    // We only shift towards the top or the right; otherwise the algorithm
    // will waste time shifting back and forth
    for (int d = 0; d < 2; d++) {
        if (touches_edge(f, d)) {
            // We can't shift in this direction
            continue;
        }

        // Make a copy and shift it in the given direction
        field fs = fcopy(f);
        shift(&fs, d);

        // Try to place a tile
        // We have to try to place one otherwise the `solve` calls will keep shifting back and forth
        // Only need to check bottom-left tiles since those are the
        // only ones we can have freed by shifting top or right
        coord frees[fs.num_tiles / 2];
        unsigned int num_free = free_cells_dir(&fs, frees, d);
        for (int i = 0; i < num_free; i++) {
            // Make another copy
            field fsp = fcopy(&fs);

            // Try to place something
            if (try_place_any(&fsp, frees[i])) {
                solve_inner(&fsp, solved, num_solved);
                if (*num_solved == MAX_SOLVED) {
                    return;
                }
            }
        }
    }
}

int solve(field* solved, field* f) {
    field fi = fcopy(f);

    // This ***must*** be placed at (0, 0) for logic to work
    place(&fi, 0, c(0, 0));
    int num_solved = 0;
    solve_inner(&fi, solved, &num_solved);
    return num_solved;
}
