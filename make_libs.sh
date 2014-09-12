#!/usr/bin/env bash
set -e

cd xbuild

premake gmake
make -j config=release
cp *.a ../lib
