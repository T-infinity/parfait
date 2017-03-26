cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitUtils)
include ( FindPackageHandleStandardArgs )
set (CMAKE_CXX_STANDARD 14)

include_directories(
    ../parfait-core/src/
    ../parfait-parallel/src/
    ../third-party-deps/tinyxml/src
    .
    dumbviz/src
)

#add_executable(UgridViewer ugrid_viewer.cpp)
add_executable(UgridHeaderPrinter ugrid_header_printer.cpp)
add_executable(UgridToSTL ugrid_to_stl.cpp)

#add_executable(UgridAnalyzer ugrid_analyzer.cpp UgridMeshAnalyzer.cpp)
#add_executable(UgridSurfaceViewer ugrid_surface_viewer.cpp)
#add_executable(ExplodeUgrid ugrid_exploder.cpp ../src/VtkUnstructuredSurfaceWriter.h ../src/VtkUnstructuredSurfaceWriter.hpp)

