#ifndef FIELD_DEF
#define FIELD_DEF

#include "../tile.h"

typedef struct {
    // The tiles related to this field, placed or not
    tile tiles[64];
    unsigned int num_tiles;
    unsigned int size;
    // Whether or not the tiles have been placed (bools)
    // Used to avoid searching through `inner`
    int placed[64];
    // The actual field - values are indexes into `tiles`
    //
    // We indicate an empty space by setting it to `f.num_tiles`
    unsigned int inner[64];
} field;

#endif
