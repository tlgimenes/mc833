#!/bin/bash

# Kill children when script finishes or 
trap 'trap - SIGTERM && kill 0' SIGINT SIGTERM EXIT

if [ $# -ne 4 ]; then
  echo "Usage: simulate.sh cars_number security_protocol entertainment_protocol comfort_protocol"
  exit 1
fi

# Executable names
server="./server_main"
client="./client_main"
hostname="localhost"

# Start servers
cmd="$server security $2 10"
$cmd & #>/dev/null
cmd="$server entertainment $3 100"
$cmd & #>/dev/null
cmd="$server comfort $4 100"
$cmd & #>/dev/null

# Start clients
cmd="$client $hostname $2 $3 $4"
for ((i = 1; i <= $1; i++)); do
  $cmd & #>/dev/null
done

# Wait for process
wait
