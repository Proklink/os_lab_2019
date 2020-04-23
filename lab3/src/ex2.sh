#!/bin/bash

gcc -c -std=c99 parallel_min_max.c find_min_max.c utils.c 
gcc parallel_min_max.o find_min_max.o utils.o  -o Parallel_Min_max
./Parallel_Min_max --array_size 10 --pnum 1 --seed 1 