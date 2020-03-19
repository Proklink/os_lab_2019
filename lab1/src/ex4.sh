#!/bin/bash


for ((i=0; i < 150; i++))
do
cat /dev/random | head -c 2 | od -An -d >> numbers.txt

done