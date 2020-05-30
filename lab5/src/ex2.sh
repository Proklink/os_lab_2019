#!/bin/bash

gcc -o MThreadFac -lpthread -std=c99 MThreadFac.c
./MThreadFac --k 10 --pnum=10 --mod=17