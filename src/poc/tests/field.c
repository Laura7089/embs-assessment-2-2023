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

field get_rubric_example_placed_asis(void) {
    field f = get_rubric_example();
    for (int i = 0; i < 4; i++) {
        f.inner[i] = i;
        f.placed[i] = 1;
    }
    return f;
}

void test_create(void) {
    field f = get_rubric_example();
    free_bufs(&f);
}

void test_idxo(void) {
    field f = get_rubric_example_placed_asis();
    TEST_ASSERT_EQUAL(idxo(&f, c(0, 0), top), 1);
    TEST_ASSERT_EQUAL(idxo(&f, c(0, 0), right), 2);
    TEST_ASSERT_EQUAL(idxo(&f, c(0, 0), bottom), 4);
    TEST_ASSERT_EQUAL(idxo(&f, c(0, 0), left), 4);

    TEST_ASSERT_EQUAL(idxo(&f, c(1, 0), top), 3);
    TEST_ASSERT_EQUAL(idxo(&f, c(1, 0), right), 4);
    TEST_ASSERT_EQUAL(idxo(&f, c(1, 0), bottom), 4);
    TEST_ASSERT_EQUAL(idxo(&f, c(1, 0), left), 0);

    free_bufs(&f);
}

void test_place(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, c(0, 0));
    TEST_ASSERT_EQUAL(idx(&f, c(0, 0)), 0);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 1)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 1)), 4);
    TEST_ASSERT(f.placed[0]);

    unplace(&f, c(0, 0));
    TEST_ASSERT_EQUAL(idx(&f, c(0, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 1)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 1)), 4);
    TEST_ASSERT(!f.placed[0]);

    free_bufs(&f);
}

void test_touches_edge(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, c(0, 0));

    TEST_ASSERT(touches_edge(&f, bottom));
    TEST_ASSERT(touches_edge(&f, left));
    TEST_ASSERT(!touches_edge(&f, top));
    TEST_ASSERT(!touches_edge(&f, right));

    free_bufs(&f);
}

void test_shift(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, c(0, 0));

    shift(&f, top);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 1)), 0);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 1)), 4);

    shift(&f, right);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 1)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 1)), 0);

    shift(&f, bottom);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 1)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 0)), 0);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 1)), 4);

    shift(&f, left);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 0)), 0);
    TEST_ASSERT_EQUAL(idx(&f, c(0, 1)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 0)), 4);
    TEST_ASSERT_EQUAL(idx(&f, c(1, 1)), 4);

    free_bufs(&f);
}

void test_fits(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, c(0, 0));

    // Test fits in tile (0, 1)
    // Tile 3 doesn't fit, the other two do
    TEST_ASSERT(tile_fits(&f, 1, c(0, 1)));
    TEST_ASSERT_EQUAL(0, f.tiles[1].rotation);
    TEST_ASSERT(tile_fits(&f, 2, c(0, 1)));
    TEST_ASSERT_EQUAL(2, f.tiles[2].rotation);
    TEST_ASSERT(!tile_fits(&f, 3, c(0, 1)));
    // Test fits in tile (1, 0)
    // None can fit because none have blue
    TEST_ASSERT(!tile_fits(&f, 1, c(1, 0)));
    TEST_ASSERT(!tile_fits(&f, 2, c(1, 0)));
    TEST_ASSERT(!tile_fits(&f, 3, c(1, 0)));
    // Test fits in tile (1, 1)
    // All should fit because nothing borders this space
    TEST_ASSERT(tile_fits(&f, 1, c(1, 1)));
    TEST_ASSERT_EQUAL(0, f.tiles[1].rotation);
    TEST_ASSERT(tile_fits(&f, 2, c(1, 1)));
    TEST_ASSERT_EQUAL(2, f.tiles[2].rotation);
    TEST_ASSERT(tile_fits(&f, 3, c(1, 1)));
    TEST_ASSERT_EQUAL(0, f.tiles[3].rotation);

    field f2 = get_rubric_example();
    rotate_right(&f2.tiles[3]);
    rotate_right(&f2.tiles[3]);
    place(&f2, 3, c(0, 0));
    print_field(&f2);
    TEST_ASSERT(tile_fits(&f, 2, c(0, 1)));
    TEST_ASSERT_EQUAL(3, f.tiles[2].rotation);

    free_bufs(&f);
    free_bufs(&f2);
}

void test_free_spaces(void) {
    field f = get_rubric_example();
    // Place the first tile in the bottom-left corner
    place(&f, 0, c(0, 0));

    coord free[4] = { c(10, 10), c(10, 10), c(10, 10), c(10, 10) };
    unsigned int num_free = free_cells(&f, free);

    TEST_ASSERT_EQUAL(num_free, 2);
    TEST_ASSERT_EQUAL(free[0].x, 0);
    TEST_ASSERT_EQUAL(free[0].y, 1);
    TEST_ASSERT_EQUAL(free[1].x, 1);
    TEST_ASSERT_EQUAL(free[1].y, 0);
    TEST_ASSERT_EQUAL(free[2].x, 10);
    TEST_ASSERT_EQUAL(free[2].y, 10);
    TEST_ASSERT_EQUAL(free[3].x, 10);
    TEST_ASSERT_EQUAL(free[3].y, 10);

    free_bufs(&f);
}

void test_sur_colours(void) {
    {
        field f = get_rubric_example_placed_asis();
        colour ncols[4];

        unplace(&f, c(1, 1));
        sur_colours(&f, ncols, c(1, 1));
        colour expected[] = {127, 127, 0, 4};
        TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, ncols, 4);

        free_bufs(&f);
    }

    {
        field f = get_rubric_example_placed_asis();
        colour ncols[4];

        unplace(&f, c(0, 0));
        sur_colours(&f, ncols, c(0, 0));
        colour expected[] = {0, 5, 127, 127};
        TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, ncols, 4);

        free_bufs(&f);
    }

    {
        field f = get_rubric_example();
        rotate_right(&f.tiles[3]);
        rotate_right(&f.tiles[3]);
        place(&f, 3, c(0, 0));

        colour ncols[4];
        sur_colours(&f, ncols, c(0, 1));
        colour expected[] = {127, 127, 5, 127};
        TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, ncols, 4);

        colour ncols2[4];
        sur_colours(&f, ncols2, c(1, 0));
        colour expected2[] = {127, 127, 127, 2};
        TEST_ASSERT_EQUAL_CHAR_ARRAY(expected2, ncols2, 4);

        free_bufs(&f);
    }
}

void test_try_place(void) {
    field f = get_rubric_example();
    rotate_right(&f.tiles[3]);
    rotate_right(&f.tiles[3]);
    place(&f, 3, c(0, 0));
    print_field(&f);

    TEST_ASSERT(try_place(&f, 2, c(0, 1)));
    printf("\n");
    print_field(&f);
    TEST_ASSERT_EQUAL(2, idx(&f, c(0, 1)));
    TEST_ASSERT_EQUAL(3, f.tiles[2].rotation);

    free_bufs(&f);
}

void test_repr_empty(void) {
    field f = new_field(2);

    char buf[1000];
    int written = repr_field(buf, &f);
    TEST_ASSERT_EQUAL(42, written);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "eeeeee\neeeeee\neeeeee\neeeeee\neeeeee\neeeeee",
        buf,
        written
    );

    free_bufs(&f);
}

void test_repr_filled(void) {
    field f = get_rubric_example_placed_asis();

    char buf[1000];
    int written = repr_field(buf, &f);
    TEST_ASSERT_EQUAL(42, written);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        " 0  3 \n3 42 4\n 0  5 \n 0  0 \n0 15 3\n 2  4 ",
        buf,
        written
    );

    free_bufs(&f);
}
