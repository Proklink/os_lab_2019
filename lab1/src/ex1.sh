#! /bin/bash

./background.sh &
touch file
echo "i like computers" > file
cat file
clear
wc -m file