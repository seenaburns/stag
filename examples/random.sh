#!/bin/bash
FREQUENCY=0.5

while true;
do
    echo $[ RANDOM % 100 ];
    sleep $FREQUENCY;
done | ./stag --title "Random Data Over Time" "$@"
