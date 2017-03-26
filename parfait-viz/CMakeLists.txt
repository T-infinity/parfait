cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitViz)
include ( FindPackageHandleStandardArgs )
set (CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)

add_definitions(
  -DPARFAIT_WITH_MPI
  -DPARFAIT_WITH_PARMETIS
  -DCATCH_WITH_MPI
)

include_directories(
        ../parfait-core/src/
        src
        test
        ${MPI_INCLUDE_PATH}
)

file(GLOB_RECURSE PARFAIT_VIZ_HEADERS
        src/*.h
        src/*.hpp
)

add_library(ParfaitViz src/VizPluginLoader.cpp)

add_subdirectory(plugins)

install(FILES ${PARFAIT_VIZ_HEADERS} 
    DESTINATION parfait-viz/include)

install(TARGETS ParfaitViz 
    EXPORT parfait-viz-targets
    DESTINATION parfait-viz/lib)

install(FILES parfait-vizConfig.cmake 
    DESTINATION parfait-viz)
