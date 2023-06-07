#!/bin/bash
set -euxo pipefail

OUT_DIR=${1:-puzzles}
GENERATOR=${2:-./generate.py}

mkdir -p "$OUT_DIR"
for size in {2..8}; do
    for iter in {0..10}; do
        header="$OUT_DIR/size${size}_${iter}.h"
        "$GENERATOR" "$size" > "$header"
        sed -i 's/tile.h/..\/tile.h/g' "$header"
    done
done
