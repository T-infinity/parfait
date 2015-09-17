
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitVtkTests)

add_definitions(-DPARFAIT_WITH_MPI -DPARFAIT_WITH_PARMETIS -DCATCH_WITH_MPI -DPARFAIT_WITH_VTK)
find_package(MPI REQUIRED)
find_package(VTK REQUIRED)

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${MPI_INCLUDE_PATH}"
    )
include_directories(../../tinyxml/src
                    ../)

    file(GLOB_RECURSE HEADERS
        "../../src/*.h"
        "../../src/*.hpp"
        )

set(VTK_WRITING_SOURCES
        catch_main.cpp
        VtkUnstructuredWriterTests.cpp
)

include(${VTK_USE_FILE})
LIST(REMOVE_ITEM VTK_LIBRARIES
        vtkPointSpriteRendering
        vtkPointSpriteGraphics
        vtkQvis
        vtkUtilitiesPythonInitializer
        vtkEyeDomeLighting
        vtkSciberQuest
)

add_executable(VtkTests ${VTK_WRITING_SOURCES})
target_link_libraries(VtkTests
        ${MPI_LIBRARIES}
        ${VTK_LIBRARIES}
)
