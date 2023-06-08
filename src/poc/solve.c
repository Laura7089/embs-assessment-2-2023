#include <stdlib.h>
#include "solve.h"
#include "field.h"
#include "tile.h"

#include <stdio.h>

void solve_inner(field* f, field* solved, int* num_solved) {
    if (*num_solved == MAX_SOLVED) {
        return;
    }

    // printf("Solve called with:\n");
    // print_field(f);

    {
        coord frees[f->num_tiles / 2];
        unsigned int num_free = free_cells(f, frees);

        // Base case
        if (num_free == 1) {
            solved[*num_solved] = fcopy(f);
            if (try_place_any(&solved[*num_solved], frees[0])) {
                // printf("Base case found:\n");
                // print_field(&solved[*num_solved]);
                *num_solved += 1;
            }
            // Return immediately because there's no point recursing here
            return;
        }

        // Recursive Case 1: plain ol' placing
        for (int i = 0; i < num_free; i++) {
            field fs = fcopy(f);
            if (try_place_any(&fs, frees[i])) {
                solve_inner(&fs, solved, num_solved);
                if (*num_solved == MAX_SOLVED) {
                    return;
                }
            }
        }
        // printf("Recursive cases run on %d possible squares\n", num_free);
    }

    // Recursive Case 2: start shiftin'
    //
    // We only shift towards the top or the right; otherwise the algorithm
    // will waste time shifting back and forth
    if (!touches_edge(f, top)) {
        // Make a copy and shift it
        field fs = fcopy(f);
        shift(&fs, top);
        for (int x = 0; x < fs.size; x++) {
            // Skip spaces without anything beside them
            if (idxo(&fs, c(x, 0), top) == fs.num_tiles) {
                continue;
            }

            // Make another copy
            field fsp = fcopy(&fs);

            // Try to place something
            if (try_place_any(&fsp, c(x, 0))) {
                solve_inner(&fsp, solved, num_solved);
                if (*num_solved == MAX_SOLVED) {
                    return;
                }
            }
        }
        // printf("Shifted up\n");
    }
    if (!touches_edge(f, right)) {
        // Make a copy and shift it
        field fs = fcopy(f);
        shift(&fs, right);
        for (int y = 0; y < fs.size; y++) {
            // Skip spaces without anything beside them
            if (idxo(&fs, c(0, y), right) == fs.num_tiles) {
                continue;
            }

            // Make another copy
            field fsp = fcopy(&fs);

            // Try to place something
            if (try_place_any(&fsp, c(0, y))) {
                solve_inner(&fsp, solved, num_solved);
                if (*num_solved == MAX_SOLVED) {
                    return;
                }
            }
        }
        // printf("Shifted right\n");
    }

    // printf("End reached\n");
}

int solve(field* solved, field* f) {
    field fi = fcopy(f);

    // This ***must*** be placed at (0, 0) for logic to work
    place(&fi, 0, c(0, 0));
    int num_solved = 0;
    solve_inner(&fi, solved, &num_solved);
    return num_solved;
}
