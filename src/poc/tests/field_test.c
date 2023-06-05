#include <string.h>
#include "unity.h"
#include "../field.h"
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

field get_rubric_example(void) {
    field f = new_field(2);
    memcpy(f.tiles, tiles, 4 * sizeof(tile));
    return f;
}

void test_create(void) {
    field f = get_rubric_example();
    free_bufs(&f);
}

void test_place(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, 0, 0);
    TEST_ASSERT_EQUAL(idx(&f, 0, 0), 0);
    TEST_ASSERT_EQUAL(idx(&f, 0, 1), 4);
    TEST_ASSERT_EQUAL(idx(&f, 1, 0), 4);
    TEST_ASSERT_EQUAL(idx(&f, 1, 1), 4);
    TEST_ASSERT(f.placed[0]);

    free_bufs(&f);
}

void test_touches_edge(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, 0, 0);

    TEST_ASSERT(touches_edge(&f, bottom));
    TEST_ASSERT(touches_edge(&f, left));
    TEST_ASSERT(!touches_edge(&f, top));
    TEST_ASSERT(!touches_edge(&f, right));

    free_bufs(&f);
}

void test_shift(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, 0, 0);
    shift(&f, top);

    TEST_ASSERT_EQUAL(idx(&f, 0, 0), 4);
    TEST_ASSERT_EQUAL(idx(&f, 0, 1), 0);
    TEST_ASSERT_EQUAL(idx(&f, 1, 0), 4);
    TEST_ASSERT_EQUAL(idx(&f, 1, 1), 4);

    free_bufs(&f);
}

void test_fits(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, 0, 0);

    // Test fits in tile (0, 1)
    // Tile 3 doesn't fit, the other two do
    TEST_ASSERT(tile_fits(&f, 1, 0, 1));
    TEST_ASSERT(tile_fits(&f, 2, 0, 1));
    TEST_ASSERT(!tile_fits(&f, 3, 0, 1));
    // Test fits in tile (1, 0)
    // None can fit because none have blue
    TEST_ASSERT(!tile_fits(&f, 1, 1, 0));
    TEST_ASSERT(!tile_fits(&f, 2, 1, 0));
    TEST_ASSERT(!tile_fits(&f, 3, 1, 0));
    // Test fits in tile (1, 1)
    // All should fit because nothing borders this space
    TEST_ASSERT(tile_fits(&f, 1, 1, 1));
    TEST_ASSERT(tile_fits(&f, 2, 1, 1));
    TEST_ASSERT(tile_fits(&f, 3, 1, 1));

    free_bufs(&f);
}
