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
    // Dimensions of `inner`
    unsigned int width;
    unsigned int height;
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
field new_field(unsigned int num_tiles, unsigned int width, unsigned int height);
