#! /bin/bash

gcc -c swap.c main.c
gcc swap.o main.o -o SWAP 
./SWAP