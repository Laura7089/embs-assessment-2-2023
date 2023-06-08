#include <string.h>
#include "unity.h"
#include "../field.h"
#include "../tile.h"
#include "../solve.h"

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

void test_solve_rubric(void) {
    field f = get_rubric_example();

    field solved[MAX_SOLVED];
    int num_solved = solve(solved, &f);

    TEST_ASSERT(solved);
}
