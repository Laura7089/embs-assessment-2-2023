#include "field.h"

// Find a solution to the puzzle.
//
// Expects at least one tile to be placed.
// If it finds a solution, mutates `f` and returns 1;
// If not, return 0 and the final state of `f` is unknown.
int solve(field* f);
