#!/bin/bash

if [ "$1" == "" ]; then
	echo "Usage: ./run.sh </path/to/program/>"
	exit
fi

# copy from global config to 15 different config file
cp sample.conf pro_0.conf
echo -e "log:log/process_0_log.txt\nid:0" >> pro_0.conf
cp sample.conf pro_1.conf
echo -e "log:log/process_1_log.txt\nid:1" >> pro_1.conf
cp sample.conf pro_2.conf
echo -e "log:log/process_2_log.txt\nid:2" >> pro_2.conf
cp sample.conf pro_3.conf
echo -e "log:log/process_3_log.txt\nid:3" >> pro_3.conf
cp sample.conf pro_4.conf
echo -e "log:log/process_4_log.txt\nid:4" >> pro_4.conf
cp sample.conf pro_5.conf
echo -e "log:log/process_5_log.txt\nid:5" >> pro_5.conf
cp sample.conf pro_6.conf
echo -e "log:log/process_6_log.txt\nid:6" >> pro_6.conf
cp sample.conf pro_7.conf
echo -e "log:log/process_7_log.txt\nid:7" >> pro_7.conf
cp sample.conf pro_8.conf
echo -e "log:log/process_8_log.txt\nid:8" >> pro_8.conf
cp sample.conf pro_9.conf
echo -e "log:log/process_9_log.txt\nid:9" >> pro_9.conf
cp sample.conf pro_10.conf
echo -e "log:log/process_10_log.txt\nid:10" >> pro_10.conf
cp sample.conf pro_11.conf
echo -e "log:log/process_11_log.txt\nid:11" >> pro_11.conf
cp sample.conf pro_12.conf
echo -e "log:log/process_12_log.txt\nid:12" >> pro_12.conf
cp sample.conf pro_13.conf
echo -e "log:log/process_13_log.txt\nid:13" >> pro_13.conf
cp sample.conf pro_14.conf
echo -e "log:log/process_14_log.txt\nid:14" >> pro_14.conf
echo "Created temporary config files.\n"

# execute all processes in background
echo "Excuting processes in background..."
$1 > /dev/null pro_0.conf &
pid0=$!
$1 > /dev/null pro_1.conf &
pid1=$!
$1 > /dev/null pro_2.conf &
pid2=$!
$1 > /dev/null pro_3.conf &
pid3=$!
$1 > /dev/null pro_4.conf &
pid4=$!
$1 > /dev/null pro_5.conf &
pid5=$!
$1 > /dev/null pro_6.conf &
pid6=$!
$1 > /dev/null pro_7.conf &
pid7=$!
$1 > /dev/null pro_8.conf &
pid8=$!
$1 > /dev/null pro_9.conf &
pid9=$!
$1 > /dev/null pro_10.conf &
pid10=$!
$1 > /dev/null pro_11.conf &
pid11=$!
$1 > /dev/null pro_12.conf &
pid12=$!
$1 > /dev/null pro_13.conf &
pid13=$!
$1 > /dev/null pro_14.conf &
pid14=$!
echo "Executed all processes."

# wait for all processes to change signal handler
echo "Sleep for 2 seconds."
sleep 2

# notice processes to start
echo "Sending signals to processes..."
kill -s USR1 $pid0
kill -s USR1 $pid1
kill -s USR1 $pid2
kill -s USR1 $pid3
kill -s USR1 $pid4
kill -s USR1 $pid5
kill -s USR1 $pid6
kill -s USR1 $pid7
kill -s USR1 $pid8
kill -s USR1 $pid9
kill -s USR1 $pid10
kill -s USR1 $pid11
kill -s USR1 $pid12
kill -s USR1 $pid13
kill -s USR1 $pid14
echo "All signals sent."

# wait for all processes to finish
echo "Waiting for all processes to finish..."
wait $pid0
wait $pid1
wait $pid2
wait $pid3
wait $pid4
wait $pid5
wait $pid6
wait $pid7
wait $pid8
wait $pid9
wait $pid10
wait $pid11
wait $pid12
wait $pid13
wait $pid14
echo "All processes finished."

# remove all temporary config file
rm pro_0.conf
rm pro_1.conf
rm pro_2.conf
rm pro_3.conf
rm pro_4.conf
rm pro_5.conf
rm pro_6.conf
rm pro_7.conf
rm pro_8.conf
rm pro_9.conf
rm pro_10.conf
rm pro_11.conf
rm pro_12.conf
rm pro_13.conf
rm pro_14.conf
echo "Removed all temporary config files."
