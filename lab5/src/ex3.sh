#!/bin/bash

gcc -o deadlock -lpthread -std=c99 deadlock.c
./deadlock 