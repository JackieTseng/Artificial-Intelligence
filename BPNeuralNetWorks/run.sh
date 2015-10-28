#!/bin/sh

if [ $# -eq 0 ]; then
    ./bin/BPNetWork
elif [ $# -eq 1 ]; then
    ./bin/BPNetWork $1
elif [ $# -eq 2 ]; then
    ./bin/BPNetWork $1 $2
elif [ $# -eq 3 ]; then
    ./bin/BPNetWork $1 $2 $3
elif [ $# -eq 4 ]; then
    ./bin/BPNetWork $1 $2 $3 $4
else 
    echo "Parameter Error!"
fi
