#!/bin/bash

if [ "$1" == "" ]; then
	echo "Usage: ./autorun_4p.sh </path/to/program/>"
	exit
fi

echo "Excuting processes in background..."
$1 > /dev/null 4p_0.conf &
pid1=$!
$1 > /dev/null 4p_1.conf &
pid2=$!
$1 > /dev/null 4p_2.conf &
pid3=$!
$1 > /dev/null 4p_3.conf &
pid4=$!
echo "Executed all processes."

# wait for all processes to set signal handler
echo "Sleep for 1 second."
sleep 1

echo "Sending signals to processes..."
kill -s USR1 $pid1
kill -s USR1 $pid2
kill -s USR1 $pid3
kill -s USR1 $pid4
echo "All signals sent."

echo "Waiting for all processes to finish..."
wait $pid1
wait $pid2
wait $pid3
wait $pid4
echo "All processes finished."
