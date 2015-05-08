#!/usr/bin/env bash
set -e

cd test
mkdir -p build
cd build
cmake ../../
make -j
./parfait_test

