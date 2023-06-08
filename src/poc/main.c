#include <stdio.h>
#include <string.h>
#include "tile.h"
#include "field.h"
#include "solve.h"

tile RUBRIC_TILES[] = {
    {{ 0, 1, 2, 0 }, 0},
    {{ 0, 4, 0, 3 }, 0},
    {{ 0, 3, 4, 5 }, 0},
    {{ 3, 4, 5, 2 }, 0},
};

#include "./puzzles/size3_6.h"

int main(int argc, char** argv) {
    field f = new_field(NTILESROOT);
    memcpy(f.tiles, TILES, NTILES * sizeof(tile));
    /* memcpy(f.tiles, TILES, NTILES * sizeof(tile)); */

    field solved[MAX_SOLVED];
    int num_solved = solve(solved, &f);
    if (!num_solved) {
        return 1;
    }

    printf("Solution found:\n");
    print_field(&solved[0]);

    free_bufs(&f);
    for (int i = 0; i < num_solved; i++) {
        free_bufs(&solved[i]);
    }
    return 0;
}
