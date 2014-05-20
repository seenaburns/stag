#!/bin/bash

EXECUTABLE=../stag
OUT=/dev/null

if [[ -n "$1" ]]
then
    OUT=$1
fi

while true;
do
    echo $[ RANDOM % 100 ]
done | tee $OUT | $EXECUTABLE
