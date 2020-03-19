#!/bin/bash
exec 0< numbers.txt
count=0
sum=0
while read word
do
sum=$(($sum + $word))
count=$(( $count + 1 ))
done
sr_arf=0
let "sr_arf = sum / count"
echo "среднее арифметическое $sr_arf" 
echo "количество элементов $count" 