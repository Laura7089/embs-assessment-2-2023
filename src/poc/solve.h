#include "field.h"

#define MAX_SOLVED 8

// Find a solution to the puzzle.
//
// Writes solved fields to `solved`, returns number written.
// If no solutions, noop and return 0.
int solve(field* solved, field* f);
