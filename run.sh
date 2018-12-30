#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/multiverse
else
    ./bin/debug/multiverse
fi