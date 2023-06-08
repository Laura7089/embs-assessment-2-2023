#include <string.h>
#include "unity.h"
#include "../tile.h"

// Rubric image example
// lilac = 0
// blue = 1
// green = 2
// yellow = 3
// red = 4
// cyan = 5
tile tiles[] = {
    {{ 0, 1, 2, 0 }, 0},
    {{ 0, 4, 0, 3 }, 0},
    {{ 0, 3, 4, 5 }, 0},
    {{ 3, 4, 5, 2 }, 0},
};

void setUp(void) {}
void tearDown(void) {}

void test_get_side(void) {
    tile t = tiles[0];
    TEST_ASSERT_EQUAL(2, get_side(t, bottom));
    TEST_ASSERT_EQUAL(1, get_side(t, right));

    rotate_right(&t);
    TEST_ASSERT_EQUAL(1, get_side(t, bottom));
    TEST_ASSERT_EQUAL(0, get_side(t, right));
}

void test_tile_repr(void) {
    tile t = tiles[0];

    char buf[100];
    repr_tile(buf, t);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buf, " 0 \n0 1\n 2 ", 12);
}
