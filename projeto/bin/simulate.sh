#!/bin/bash

# Kill children when script finishes or 
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT

if [ $# -ne 4 ] ; then
  echo "Usage: simulate.sh cars_number security_protocol entertainment_protocol comfort_protocol"
fi

server="./server_main"
cmd="$server security $2 10"
$cmd & #>/dev/null
cmd="$server entertainment $3 100"
$cmd & #>/dev/null
cmd="$server comfort $4 100"
$cmd & #>/dev/null

# wait for process
wait
