#!/bin/bash
OUT=/dev/null
FREQUENCY=0.5

if [[ -n "$1" ]]
then
    OUT=$1
fi

while true;
do
    rand=$[ RANDOM % 5 ]
    if [ $rand -eq 0 ]
    then
        echo 1.0
    else
        echo 0.0
    fi
    sleep $FREQUENCY;
done | tee $OUT | ./stag --title "Random Binary Data"
