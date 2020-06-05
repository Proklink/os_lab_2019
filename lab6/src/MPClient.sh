#!/bin/bash

gcc -o MPClient -lpthread -std=c99 MPClient.c
./MPClient  --k 10 --mod 7 --servers servers