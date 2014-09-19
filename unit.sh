#!/usr/bin/env bash
set -e

cd test/serial_tests

premake gmake
make config=release clean
make -j config=release
./ALL_PARFAIT_TESTS.x
