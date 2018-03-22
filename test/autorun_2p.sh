#!/bin/bash

if [ "$1" == "" ]; then
	echo "Usage: ./autorun_2p.sh </path/to/program/>"
	exit
fi

echo "Excuting processes in background..."
$1 > /dev/null 2p_0.conf &
pid1=$!
$1 > /dev/null 2p_1.conf &
pid2=$!
echo "Executed all processes."

# wait for all processes to change signal handler
echo "Sleep for 1 second."
sleep 1

# notice processes to start
echo "Sending signals to processes..."
kill -s USR1 $pid1
kill -s USR1 $pid2
echo "All signals sent."

# wait for all processes to finish
echo "Waiting for all processes to finish..."
wait $pid1
wait $pid2
echo "All processes finished."
