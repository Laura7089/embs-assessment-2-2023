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

#include "./puzzles/size4_2.h"

int main(int argc, char** argv) {
    field f = new_field(NTILESROOT);
    memcpy(f.tiles, TILES, NTILES * sizeof(tile));
    place(&f, 0, c(0, 0)); // Place a tile to get it started

    int solved = solve(&f);
    printf("Solution found: %d\n", solved);

    free_bufs(&f);
    return 0;
}
