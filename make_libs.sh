#!/usr/bin/env bash
mkdir lib
set -e

cd xbuild

premake gmake
make -j config=release
cp *.a ../lib/
