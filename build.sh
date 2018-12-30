#!/bin/bash

mkdir -p bin
cd bin 
mkdir -p release
mkdir -p debug

if [ "$1" = "release" ]
then
    cd release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make
else
    cd debug 
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make
fi