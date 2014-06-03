#!/bin/bash
OUT=/dev/null
FREQUENCY=0.5

if [[ -n "$1" ]]
then
    OUT=$1
fi

while true;
do
    echo $[ RANDOM % 100 ];
    sleep $FREQUENCY;
done | tee $OUT | ./stag --title "Random Data Over Time"
