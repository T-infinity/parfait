cmake_minimum_required(VERSION 2.6)
project(parfait)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror -Wall")
link_directories(
    ${CMAKE_LIBRARY_PATH}
    )

include_directories(
  ${CMAKE_INCLUDE_PATH}
  src
  utils
  )

option(PARFAIT_WITH_MPI "Configure with MPI" ON)
option(PARFAIT_WITH_PARMETIS "Configure with ParMetis" ON)
option(PARFAIT_WITH_VTK "Configure with ParMetis" ON)

add_subdirectory(utils)
add_subdirectory(src/c_interfaces)
add_subdirectory(tests/serial_tests)
add_subdirectory(tests/plugin_tests)
add_subdirectory(tinyxml)
add_subdirectory(plugins)

if(PARFAIT_WITH_MPI)
  add_subdirectory(tests/parallel_tests)
endif()

if(PARFAIT_WITH_VTK)
  add_subdirectory(tests/vtk_tests)
endif()
