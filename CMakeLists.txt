cmake_minimum_required(VERSION 2.6)
project(parfait)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror -Waddress")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wchar-subscripts -Wenum-compare")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wcomment")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wformat -Wmain -Wmaybe-uninitialized")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wnonnull -Wopenmp-simd -Wparentheses")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wreorder -Wsequence-point -Wstrict-aliasing")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wunused-function -Wunused-label -Wunused-value")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wunused-variable -Wvolatile-register-var -Wreturn-type")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wsign-compare")
#          -Wunknown-pragmas
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
