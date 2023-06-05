#!/bin/env python3

import random
import sys

NUM_COLOURS = 9


def colour_grid(size):
    return [[random.randint(0, NUM_COLOURS) for _ in range(0, size)]
            for _ in range(0, size)]


def uptohalf(m, i):
    if (i * 2 < m):
        return i
    else:
        return m - i - 1


def diag_offsets(size):
    size = (size * 2) - 1
    for y in range(0, size):
        off = uptohalf(size, y)
        lower = (size // 2) - off
        upper = (size // 2) + off
        for x in range(lower, upper + 1, 2):
            yield (x, y)


def slice_diag(grid, size):
    tiles = []
    for x, y in diag_offsets(size):
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
    return f"tile TILES[{len(tiles)}] = {tile_list};\nunsigned int NTILES = {len(tiles)};"


if __name__ == "__main__":
    puzzle_size = int(sys.argv[1])
    grid = colour_grid(puzzle_size * 2)
    tiles = slice_diag(grid, puzzle_size)
    print(c_literal(tiles))
