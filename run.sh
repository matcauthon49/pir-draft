#!/bin/bash

    ./src/build/c &
    sleep 1
    ./src/build/p0 &
    sleep 3
    ./src/build/p1 &
    sleep 3
    ./src/build/p2 &
    wait