
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitParallelTests)

add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

add_definitions(-DPARFAIT_WITH_MPI -DPARFAIT_WITH_PARMETIS -DCATCH_WITH_MPI -DPARFAIT_WITH_VTK)
find_package(MPI REQUIRED)

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


set(PARFAIT_PARALLEL_TESTS_SRC
        catch_main.cpp
        ParallelExtent_test.cpp
        parallel_sum_tests.cpp
        GathervTests.cpp
        AllGathervTests.cpp
        PreProcessor/ParallelMeshReaderTests.cpp
        MessageBuilder_test.cpp
        AsyncSends_tests.cpp
        SendRecvScalarTests.cpp
        SendRecvVectorTests.cpp
        SendRecvVectorOfVectorsTests.cpp
        GatherTests.cpp
        AllGatherTests.cpp
        BroadcastTests.cpp
        ScatterTests.cpp
        ScattervTests.cpp
        ReductionTests.cpp
        MessagePasserBasicsTests.cpp
        SelfSend_test.cpp
        PreProcessor/ParallelNodeToNodeTests.cpp
        PreProcessor/NodeCenteredRedistributorTests.cpp
        PreProcessor/PreProcessorTests.cpp
       )

add_executable(ParfaitParallelTests ${PARFAIT_PARALLEL_TESTS_SRC} ${HEADERS})
target_link_libraries(ParfaitParallelTests
            ${MPI_LIBRARIES}
            tinyxml
            parmetis
            metis
            )

set(VTK_WRITING_SOURCES
        catch_main.cpp
        VtkUnstructuredWriterTests.cpp
)

find_package(VTK REQUIRED)
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
