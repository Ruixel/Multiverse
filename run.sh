#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/Subverse
else
    ./bin/debug/Subverse
fi