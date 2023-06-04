#include <stdio.h>
#include "tile.h"

// Rubric image example
// lilac = 0
// blue = 1
// green = 2
// yellow = 3
// red = 4
// cyan = 5

int main(int argc, char** argv) {
    tile t = {
        {1, 2, 3, 4},
        0,
    };

    rotate_left(&t);
    print_tile(t);

    return 0;
}
