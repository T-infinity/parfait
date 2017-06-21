#! /usr/bin/env bash
. /usr/local/pkgs/modules/init/sh
set -e

unset CMAKE_INCLUDE_PATH

module load gcc_4.9.1
module load mpt-2.11
module load ParMETIS_4.0.3-mpt-2.11-intel_2013.0.079
module load valgrind_3.9.0
module load VTK_6.3.0
export CC=gcc
export CXX=g++
export CPATH=$C_INCLUDE_PATH
export LIBRARY_PATH=$LD_LIBRARY_PATH

./unit.sh
./vtk-unit.sh
