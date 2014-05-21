#!/bin/bash

EXECUTABLE=../stag
OUT=/dev/null

FREQUENCY=0.2

if [[ -n "$1" ]]
then
    OUT=$1
fi

while true;
do
    echo $[ RANDOM % 100 ];
    sleep $FREQUENCY;
done | tee $OUT | $EXECUTABLE
