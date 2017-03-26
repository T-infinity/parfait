#!/bin/bash
set -e

cd build
make
cd parfait-core
./ParfaitSerialTests
cd ..
cd parfait-parallel
mpirun -np 1 ./ParfaitParallelTests
mpirun -np 2 ./ParfaitParallelTests
