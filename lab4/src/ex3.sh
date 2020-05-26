#!/bin/bash

gcc -c -std=c99 process_memory.c 
gcc process_memory.o -o process_memory
./process_memory 