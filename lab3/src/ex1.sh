#!/bin/bash

gcc -c sequential_min_max.c find_min_max.c utils.c 
gcc sequential_min_max.o find_min_max.o utils.o  -o Min_max
./Min_max 2 10