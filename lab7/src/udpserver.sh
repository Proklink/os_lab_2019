#!/bin/bash

gcc -o udpserver -std=c99 udpserver.c
./udpserver  --bufsize 1024 --port 20001 