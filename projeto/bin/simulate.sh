#!/bin/bash

# Kill children when script finishes or 
trap 'trap - SIGTERM && kill 0' SIGINT SIGTERM EXIT

if [ $# -ne 7 ]; then
  echo "Usage: simulate.sh cars_number security_protocol entertainment_protocol comfort_protocol security_delay entertainment_delay comfort_delay"
  exit 1
fi

# Executable names
server="./server_main"
client="./client_main"
hostname="localhost"

# Start servers
cmd="$server security $2 $5"
$cmd & #>/dev/null
cmd="$server entertainment $3 $6"
$cmd & #>/dev/null
cmd="$server comfort $4 $7"
$cmd & #>/dev/null

# Start clients
cmd="$client $hostname $2 $3 $4 $5 $6 $7"
for ((i = 1; i <= $1; i++)); do
  $cmd & #>/dev/null
done

# Wait for process
wait
