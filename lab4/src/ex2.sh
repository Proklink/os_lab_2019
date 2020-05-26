#!/bin/bash

gcc -c -std=c99 zombie.c 
gcc zombie.o -o Zombie
./Zombie 