#ifndef FIELD
#define FIELD

#include "tile.h"

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

typedef struct {
    unsigned int x;
    unsigned int y;
} coord;
#define c(x, y) (coord){ x, y }

// Index into `f.inner`, get index into `f.tiles` for the tile there
//
// Returns `f.num_tiles` if no tile is placed there
unsigned int idx(field* f, coord cs);
// Index into `f.inner`, get pointer to tile there
tile* idxt(field* f, coord cs);
// Index into `f.inner` but offset one step out from `s`
//
// Returns `f.num_tiles` if that cell is out of bounds
unsigned int idxo(field* f, coord cs, side s);

// Create/allocate a new `field`
//
// `size * size` will be assumed to be the number of tiles.
// `tiles` is ready to be filled with the puzzle tiles.
field new_field(unsigned int size);
// Get a (deep) copy of `f`
field fcopy(field* f);

// Write the 4 colours surrounding `cs` in `f` to `buf`.
//
// `top` first.
// If an adjacent space is unoccupied or out of bounds, write `127` for that space.
void sur_colours(field* f, colour* buf, coord cs);
// Check if a tile (index `t`) fits into `f` at `(x, y)`.
//
// If it does fit, this function will also rotate it appropriately.
int tile_fits(field* f, unsigned int t, coord cs);
// Count how many tiles in `f` haven't been placed yet
unsigned int num_unplaced(field* f);

// Determine if an index falls on the edge of the field
int is_edge(field* f, coord cs);
// Shift all placed tiles in the direction of the given side
//
// This is a noop if this would push any tiles out of play
void shift(field* f, side s);
// Check if the placed tiles in `f` touch the edge of the field on side `s`
int touches_edge(field* f, side s);

// Place tile index `t` onto the field at `(x, y)`
//
// If `t` is already placed, do nothing.
// This does not check if the tile will fit.
void place(field* f, unsigned int t, coord cs);
// Try to fit `t` into `cs`, in any rotation; return whether successful
//
// Returns early (with `0`) if `t` is already placed or `cs` has a tile there.
int try_place(field* f, unsigned int t, coord cs);
// As with `try_place`, but try to place *any* unplaced tile into `cs`
//
// Returns with `0` if none fit.
int try_place_any(field* f, coord cs);
// Remove the tile at `(x, y)` from `f`
//
// If there is no tile there, do nothing
void unplace(field* f, coord cs);

// Check if all tiles are placed and the field is full
int is_full(field* f);
// Get all `coord`s which are empty and bordered by at least one tile
//
// Returns length of data written to `buf`
unsigned int free_cells(field* f, coord* buf);
// Get all `coords` which are empty and bordered on side `border`
//
// Returns length of data written to `buf`
unsigned int free_cells_dir(field* f, coord* buf, side border);

// Check if the tile placements of two `fields` are identical
int placement_equal(field* l, field* r);

// Write a string representation of `f` to `buf`
//
// Returns how many characters were written.
// This value is expected to be `(f.num_tiles * 9) + (3 * f.size`).
unsigned int repr_field(char* buf, field* f);
// Print the output of `repr_field` to stdout.
//
// Writes a trailing newline.
void print_field(field* f);

#endif
