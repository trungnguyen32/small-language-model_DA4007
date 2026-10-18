#!/bin/bash

make

echo -n "ababac" > training.txt

echo "Test 1: Normal generation"
./slm 2 training.txt 10

echo "Test 2: Pseudocount smoothing"
./slm -a 0.01 2 training.txt 10

echo "Test 3: Invalid k"
./slm 0 training.txt 10

echo "Test 4: Missing file"
./slm 2 missing.txt 10