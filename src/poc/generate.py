#!/bin/env python3

import random
import sys

NUM_COLOURS = 9


def colour_grid(size):
    return [[random.randint(0, NUM_COLOURS) for _ in range(0, size)]
            for _ in range(0, size)]


def uptohalf(m, i):
    if (i * 2 < m):
        return i + 1
    else:
        return m - i


def diag_offsets(size):
    for y in range(0, size + 1):
        for x in range(0, uptohalf(size + 1, y)):
            x *= 2
            if (y % 2 == 0):
                x += 1
            yield (x, y)


def slice_diag(grid):
    diag_side = int(len(grid) / 2)
    tiles = []
    for x, y in diag_offsets(diag_side):
        top = grid[x][y]
        right = grid[x + 1][y]
        bottom = grid[x + 1][y + 1]
        left = grid[x][y + 1]

        tiles.append([top, right, bottom, left])
    return tiles


def human_repr(tile):
    return f" {tile[0]}\n{tile[3]} {tile[1]}\n {tile[2]}"


def spinr(tile):
    spin_amount = random.randint(0, 4)
    return tile[:spin_amount] + tile[spin_amount:]


def c_literal(tiles):

    def tlit(tile):
        return "{{ {{ {} }}, 0 }}".format(", ".join(map(str, tile)))

    tile_list = "{\n\t" + ",\n\t".join(map(tlit, tiles)) + "\n}"
    return f"tile tiles[{len(tiles)}] = {tile_list};\nunsigned int num_tiles = {len(tiles)};"


if __name__ == "__main__":
    puzzle_size = int(sys.argv[1])
    grid = colour_grid(puzzle_size * 2)

    tiles = slice_diag(grid)
    print("\n".join(map(human_repr, tiles)))

    print(c_literal(tiles))
