#!/usr/bin/env bash
set -e

cd test

premake gmake
make -j config=release
./ALL_PARFAIT_TESTS.x
