cmake_minimum_required(VERSION 3.6)

PROJECT(ParfaitViz)
set (CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)

add_definitions(
  -DPARFAIT_WITH_MPI
  -DPARFAIT_WITH_PARMETIS
  -DCATCH_WITH_MPI
)

set(PARFAIT_VIZ_HEADERS
        src/ParfaitVizualizer.h
        src/VtkExtentWriter.h
        src/VtkUnstructuredWriter.h
        src/VtkUnstructuredWriter.hpp
        src/VtkUnstructuredSurfaceWriter.h
        src/VtkUnstructuredSurfaceWriter.hpp
        src/VtkHacking.h
        )

install(FILES ${PARFAIT_VIZ_HEADERS} 
    DESTINATION parfait-viz/include)

install(FILES parfait-vizConfig.cmake 
    DESTINATION parfait-viz)
