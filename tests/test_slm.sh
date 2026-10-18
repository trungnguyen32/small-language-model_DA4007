#!/bin/bash

set -u

make || exit 1

printf "ababac" > training.txt

echo "Test 1: Normal generation"
./slm 2 training.txt 10

echo "Test 2: Pseudocount smoothing"
output=$(./slm -a 0.01 2 training.txt 10)

if [ "${#output}" -eq 10 ]; then
    echo "PASS: Generated 10 characters"
else
    echo "FAIL: Expected 10 characters, got ${#output}"
fi

echo "Test 3: Invalid k"
if ./slm 0 training.txt 10; then
    echo "FAIL: Invalid k was accepted"
else
    echo "PASS: Invalid k rejected"
fi

echo "Test 4: Missing file"
if ./slm 2 missing.txt 10; then
    echo "FAIL: Missing file was accepted"
else
    echo "PASS: Missing file rejected"
fi