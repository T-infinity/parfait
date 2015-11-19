add_executable(UgridSurfaceViewer ugrid_surface_viewer.cpp)

add_executable(UgridViewer ugrid_viewer.cpp)

add_executable(UgridAnalyzer ugrid_analyzer.cpp UgridMeshAnalyzer.cpp)

add_executable(ExplodeUgrid ugrid_exploder.cpp ../src/VtkUnstructuredSurfaceWriter.h ../src/VtkUnstructuredSurfaceWriter.hpp)
