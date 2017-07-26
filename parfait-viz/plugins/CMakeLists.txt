cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitVizVtkPlugin)
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

add_definitions(-DPARFAIT_WITH_MPI -DPARFAIT_WITH_PARMETIS -DPARFAIT_WITH_VTK)
find_package(MPI REQUIRED)

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${MPI_INCLUDE_PATH}"
    )

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(VtkPluginSources
        VtkPlugin.cpp
       )

add_library(ParfaitVtkPlugin SHARED
        ${VtkPluginSources}
        )
target_include_directories(ParfaitVtkPlugin PUBLIC
                ../../parfait-core/src/
        ../src
)

target_link_libraries(ParfaitVtkPlugin
        ${VTK_LIBRARIES}
        )

    #add_subdirectory(tests)
