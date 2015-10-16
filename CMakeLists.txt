cmake_minimum_required(VERSION 2.6)
project(parfait)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(
  src
  utils
  )

option(PARFAIT_WITH_MPI "Configure with MPI" ON)
option(PARFAIT_WITH_PARMETIS "Configure with ParMetis" ON)

add_subdirectory(utils)
#add_subdirectory(src/c_interfaces)
add_subdirectory(tests/serial_tests)
add_subdirectory(tinyxml)

if(PARFAIT_WITH_MPI)
  add_subdirectory(tests/parallel_tests)
endif()

if(PARFAIT_WITH_VTK)
  add_subdirectory(tests/vtk_tests)
endif()
