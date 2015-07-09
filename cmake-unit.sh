#!/usr/bin/env bash
set -e

cd tests
mkdir -p build
cd build
cmake ../../
make -j

cd tests/serial_tests
./ParfaitSerialTests

