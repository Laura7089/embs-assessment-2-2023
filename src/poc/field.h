#ifndef FIELD
#define FIELD

#include "tile.h"

typedef struct {
    // The tiles related to this field, placed or not
    tile* tiles;
    unsigned int num_tiles;
    // Whether or not the tiles have been placed (bools)
    // Used to avoid searching through `inner`
    int* placed;
    // The actual field - values are indexes into `tiles`
    //
    // We indicate an empty space by setting it to `f.num_tiles`
    unsigned int* inner;
    // Side length of `inner`
    unsigned int side;
} field;

// Index into `f.inner`, get index into `f.tiles` for the tile there
//
// Returns `f.num_tiles` if no tile is placed there
unsigned int idx(field* f, unsigned int x, unsigned int y);

// Index into `f.inner`, get pointer to tile there
tile* idxt(field* f, unsigned int x, unsigned int y);

// Create and allocate a new `field`
//
// `tiles` of the returned object is allocated but points to invalid data.
// Caller must insert tiles as needed.
field new_field(unsigned int num_tiles, unsigned int size);
// Get a (deep) copy of `f`
field copy(field* f);

// Check if a tile (index `t`) fits into `f` at `(x, y)`.
int tile_fits(field* f, unsigned int t, unsigned int x, unsigned int y);
// Count how many tiles in `f` haven't been placed yet
unsigned int num_unplaced(field* f);

// Determine if an index falls on the edge of the field
int is_edge(field* f, unsigned int x, unsigned int y);
// Shift all placed tiles in the direction of the given side
//
// This is a noop if this would push any tiles out of play
void shift(field* f, side s);
// Check if the placed tiles in `f` touch the edge of the field on side `s`
int touches_edge(field* f, side s);

// Place tile index `t` onto the field at `(x, y)`
//
// If `t` is already placed, do nothing
void place(field* f, unsigned int t, unsigned int x, unsigned int y);
// Remove the tile at `(x, y)` from `f`
//
// If there is no tile there, do nothing
void unplace(field* f, unsigned int x, unsigned int y);

#endif
