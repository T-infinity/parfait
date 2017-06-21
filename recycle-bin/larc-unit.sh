#! /usr/bin/env bash
. /usr/local/pkgs/modules/init/sh
set -e

module purge
module load gcc_4.9.1_64
module load openmpi_1.8.6_gcc
module load ParMETIS_4.0.3_openmpi_gcc 
module load valgrind_3.9.0
module load VTK_6.3.0


mkdir -p build
cd build
  cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DPARMETIS_INCLUDE_PATH=/usr/local/pkgs-modules/ParMETIS_4.0.3-openmpi_1.8.6_gcc/include -DPARMETIS_LIBRARY=/usr/local/pkgs-modules/ParMETIS_4.0.3-openmpi_1.8.6_gcc/lib/libparmetis.a -DMETIS_LIBRARY=/usr/local/pkgs-modules/ParMETIS_4.0.3-openmpi_1.8.6_gcc/lib/libmetis.a ..
  make -j 
cd ..

./unit.sh
