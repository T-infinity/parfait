
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitInterfaces)
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

add_definitions(-DPARFAIT_WITH_MPI -DPARFAIT_WITH_PARMETIS)
find_package(MPI REQUIRED)

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${MPI_INCLUDE_PATH}"
    )

set(CMAKE_EXE_LINKER_FLAGS -L/ump/fldmd/home/cdruyorj/cdruyor_local/parmetis_openmpi/lib)
set(PARFAIT_INTERFACE_SRCS
    parfait_preprocessor.cpp
    parfait_preprocessor.h
       )

include_directories(../../tinyxml/src)
add_library(parfait ${PARFAIT_INTERFACE_SRCS})

#add_executable(example_grid_import example_grid_import.c)
#set_target_properties(example_grid_import PROPERTIES LINKER_LANGUAGE C)
#target_link_libraries(example_grid_import
#            parfait
#            tinyxml
#            ${MPI_LIBRARIES}
#            parmetis
#            metis
#            )