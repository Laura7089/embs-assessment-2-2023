#include <stdio.h>
#include "tile.h"

int main(int argc, char** argv) {
    colour t_colours[] = {1, 2, 3, 4};
    tile t;
    t.rotation = 0;
    t.colours = t_colours;

    rotate_left(&t);
    print_tile(t);

    return 0;
}
