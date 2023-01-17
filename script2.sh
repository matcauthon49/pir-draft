#!/bin/bash

# for i in {1..5};
# do
    ./src/build/c &
    sleep 1
    ./src/build/p0 &
    sleep 1
    ./src/build/p1 &
    sleep 1
    ./src/build/p2 &
    wait
# done