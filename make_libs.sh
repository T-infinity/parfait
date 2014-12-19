#!/usr/bin/env bash
mkdir lib
mkdir include
set -e

cp src/*.h* include
cd xbuild

premake gmake
make -j config=release
cp *.a ../lib/
