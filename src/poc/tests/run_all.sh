#!/bin/bash

METHODS=(
    "malloc"
    "heapless"
)

TESTS=(
    "field"
    "solve"
    "tile"
)

for method in "${METHODS[@]}"; do
    echo "Testing with $method"
    for testname in "${TESTS[@]}"; do
        echo "> Testing $testname"
        make clean >/dev/null
        make "METHOD=$method" "test_$testname" >/dev/null
        out="$("./test_$testname")"
        if [[ $? != 0 ]]; then
            echo "> FAILED:"
            echo "$out"
        fi
    done
done
