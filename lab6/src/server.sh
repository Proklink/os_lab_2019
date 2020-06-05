#!/bin/bash

gcc -o server -lpthread -std=c99 server.c
./server  --port 20001 --tnum 4