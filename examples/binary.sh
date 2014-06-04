#!/bin/bash
FREQUENCY=0.5

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
done | ./stag --title "Random Binary Data" "$@"
