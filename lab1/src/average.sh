#!/bin/bash
len=$#
echo "Total number of arguments: $len"
sum=0
for val in $@
do
sum=$((sum + val))
done
average=$((sum / len))
echo "Average: $average" 
