#!/bin/bash

gcc -c main.c revert_string.c 
gcc main.o revert_string.o -o Revert_string
./Revert_string "Eclipse Che"