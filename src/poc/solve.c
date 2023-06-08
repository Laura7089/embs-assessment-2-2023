#include <stdlib.h>
#include "solve.h"
#include "field.h"
#include "tile.h"

#include <stdio.h>

int already_solved(field* f, field* solved, int num_solved) {
    for (int i = 0; i < num_solved; i++) {
        if (placement_equal(f, &solved[i])) {
            return 1;
        }
    }

    return 0;
}

// Get the next cell that should be filled
//
// If the grid is filled, returns an oob coord
coord next_free(field* f) {
    for (int x = 0; x < f->size; x++) {
        for (int y = 0; y < f->size; y++) {
            coord cs = c(x, y);
            if (idx(f, cs) == f->num_tiles) {
                return cs;
            }
        }
    }
    return c(f->size, f->size);
}

void solve_inner(field* f, field* solved, int* num_solved, int max) {
    if (*num_solved == max) {
        return;
    }

    /* printf("Solve called with:\n"); */
    /* print_field(f); */

    {
        coord nf = next_free(f);
        /* printf("Next free spot: (%d, %d)\n", nf.x, nf.y); */

        // Base case
        if (num_unplaced(f) == 1) {
            solved[*num_solved] = fcopy(f);
            if (try_place_any(&solved[*num_solved], nf)) {
                if (!already_solved(&solved[*num_solved], solved, *num_solved)) {
                    /* printf("Base case found:\n"); */
                    /* print_field(&solved[*num_solved]); */
                    *num_solved += 1;
                }
            }
            // Return immediately because there's no point recursing here
            /* printf("Returning, there's nothing to do\n"); */
            return;
        }

        // Recursive Case 1: plain ol' placing
        for (int t = 0; t < f->num_tiles; t++) {
            field fs = fcopy(f);
            if (try_place(&fs, t, nf)) {
                solve_inner(&fs, solved, num_solved, max);
                if (*num_solved == max) {
                    return;
                }
            }
        }
    }

    // No shift if the field is empty.
    if (num_unplaced(f) == f->num_tiles) {
        /* printf("Skipping shifts"); */
        return;
    }

    // Recursive Case 2: start shiftin'
    //
    // Only shift towards the top or the right.
    for (int d = 0; d < 2; d++) {
        if (touches_edge(f, d)) {
            continue;
        }

        // Make a copy and shift it
        field fs = fcopy(f);
        shift(&fs, d);
        coord nf = next_free(&fs);
        /* printf("Shifted %d\n", d); */
        // Try to place something
        for (int t = 0; t < fs.num_tiles; t++) {
            if (try_place(&fs, t, nf)) {
                solve_inner(&fs, solved, num_solved, max);
                if (*num_solved == max) {
                    return;
                }
            }
        }
    }

    /* printf("End reached\n\n"); */
}


int solve(field* solved, field* f) {
    field fi = fcopy(f);

    int num_solved = 0;
    solve_inner(&fi, solved, &num_solved, MAX_SOLVED);
    return num_solved;
}

field solve_to_first(field* f) {
    field fi = fcopy(f);

    field solved;
    int num_solved = 0;
    solve_inner(&fi, &solved, &num_solved, 1);
    return solved;
}
