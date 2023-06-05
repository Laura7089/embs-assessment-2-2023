#ifndef TILE
#define TILE

typedef unsigned char colour;

typedef struct {
    colour colours[4];
    char rotation;
} tile;

typedef enum {
    top = 0,
    right = 1,
    bottom = 2,
    left = 3,
} side;

// Rotate a tile.
//
// Mutates the argument.
void rotate_left(tile* t);
void rotate_right(tile* t);

// TODO: should the below functions take pointers to tiles??

// Check if `base` matches `other` on base's side `s`
int matches(tile base, tile other, side s);

// Find the colour on the given `side` of `t`
colour get_side(tile t, side s);

// Determine if the sequence `colours` occurs on `t`
//
// `num_colours` should be the length of `colours`.
int has_cseq(tile t, colour* colours, unsigned int num_colours);

// Write a representation of `t` to stdout, top first
void print_tile(tile t);

#endif
