#!/bin/bash

gcc -c -std=c99 mutex.c 
gcc mutex.o -lpthread -o mutex
./mutex 