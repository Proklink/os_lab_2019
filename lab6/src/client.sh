#!/bin/bash

gcc -o client -lpthread -std=c99 client.c
./client  --k 10 --mod 3 --servers servers