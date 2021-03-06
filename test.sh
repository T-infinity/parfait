#!/usr/bin/env bash
set -e

cd build
cd parfait
mpirun -np 1 ./ParfaitTests -d yes
mpirun -np 2 ./ParfaitTests -d yes
cd ..
cd MessagePasser
mpirun -np 1 ./MessagePasserTests
mpirun -np 2 ./MessagePasserTests
