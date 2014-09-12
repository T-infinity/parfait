#!/usr/bin/env bash
set -e

cd test/serial_tests

premake gmake
make -j config=release
./ALL_PARFAIT_TESTS.x
