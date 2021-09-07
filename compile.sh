#!/bin/bash

rm -r -v build
mkdir build 
cd build
cmake ..
make
