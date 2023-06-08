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

#include "./puzzles/size3_2.h"

int main(int argc, char** argv) {
    field f = new_field(2);
    memcpy(f.tiles, RUBRIC_TILES, 4 * sizeof(tile));
    /* memcpy(f.tiles, TILES, NTILES * sizeof(tile)); */
    place(&f, 0, c(0, 0)); // Place a tile to get it started

    int solved = solve(&f);
    if (!solved) {
        return 1;
    }

    printf("Solution found:\n");
    print_field(&f);

    for (int i = 0; i < 2; i++) {
        printf("\nTile at (%d, %d):\n", i, i);
        print_tile(*idxt(&f, c(i, i)));
    }

    free_bufs(&f);
    return 0;
}
