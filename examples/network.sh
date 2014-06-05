#!/bin/bash
FREQUENCY=2

# Use network statistics to get a sample every two seconds of average kbps

downspeed()
{
    rx1=$(cat /sys/class/net/wlan0/statistics/rx_bytes)
    sleep $FREQUENCY
    rx2=$(cat /sys/class/net/wlan0/statistics/rx_bytes)
    rxdiff=$(echo "$rx2 - $rx1" | bc -l)
    rxtrue=$(echo "$rxdiff/1024/$FREQUENCY" | bc -l)

    printf "%.0f\n" "$rxtrue"
}

while true;
do
    downspeed
done | ./stag --title "Download kbps" "$@"
