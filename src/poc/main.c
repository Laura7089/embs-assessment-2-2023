#include <stdio.h>
#include "tile.h"
#include "field.h"

int main(int argc, char** argv) {
    tile rubric_tiles[] = {
        {{ 0, 1, 2, 0 }, 0},
        {{ 0, 4, 0, 3 }, 0},
        {{ 0, 3, 4, 5 }, 0},
        {{ 3, 4, 5, 2 }, 0},
    };

    field f = new_field(2);
    f.tiles = rubric_tiles;
    place(&f, 0, c(0, 0)); // Place a tile to get it started

    int solved;
    solve(&f, &solved);

    printf("Solved: %d\n", solved);

    free_bufs(&f);
    return 0;
}
