#include <stdio.h>
#include "tile.h"

// Rotation is just an offset to array indexing
colour get_side(tile t, side s) {
    side wanted = (s + t.rotation) % 4;
    return t.colours[wanted];
}
void rotate_left(tile* t) {
    t->rotation += 1;
}
void rotate_right(tile* t) {
    t->rotation -= 1;
}

int matches(tile base, tile other, side s) {
    side side_of_other = (s + 2) % 4;
    return (get_side(base, s) == get_side(other, side_of_other));
}

void print_tile(tile t) {
    printf( "[%u, %u, %u, %u]",
            get_side(t, top),
            get_side(t, right),
            get_side(t, bottom),
            get_side(t, left)
          );
}

int has_cseq(tile t, colour* colours, unsigned int num_colours) {
    // Start from each side
    for (int s = 0; s < 4; s++) {
        int valid = 1;

        for (int i = 0; i < num_colours; i++) {
            if (get_side(t, s + i) != colours[i]) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            return 1;
        }
    }

    return 0;
}

side opposite(side s) {
    switch (s) {
    case top:
        return bottom;
    case right:
        return left;
    case bottom:
        return top;
    case left:
        return right;
    }
}
