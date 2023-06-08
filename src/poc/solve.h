#include "field.h"

#define MAX_SOLVED 8

// Find a solution to the puzzle.
//
// Writes solved fields to `solved`, returns number written.
// If no solutions, noop and return 0.
// `f` must have NO PLACED TILES.
int solve(field* solved, field* f);

// Find one solution and return
field solve_to_first(field* f);
