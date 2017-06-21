cmake_minimum_required(VERSION 2.8)
project(DumbVtkTests)
include ( FindPackageHandleStandardArgs )
set (CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE HEADERS
  ../parfait-core/src/*.h
  ../parfait-core/src/*.hpp
  src/*.h
  src/*.hpp
)

set(TEST_SOURCES
  test/AllTests.cpp
  test/VtkGenericTests.cpp
  test/VtkSurfaceTests.cpp
  test/VtkFieldTests.cpp
  test/VtkSliceWriterTests.cpp
  test/VtkToCGNSTests.cpp
)

include_directories(../parfait-core/src src ../third-party-deps/catch)

add_executable(DumbVtkTests ${TEST_SOURCES})
target_link_libraries(DumbVtkTests ${VTK_LIBRARIES})
