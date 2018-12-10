cmake_minimum_required(VERSION 3.0)

PROJECT(parfait-core-unit-tests)
set (CMAKE_CXX_STANDARD 14)


file(GLOB_RECURSE PARFAIT_HEADERS
    "*.h"
    "*.hpp"
    )


if(BUILD_TESTS)
    set(SERIAL_TESTS_SOURCES
        test/AllTests.cpp
        test/Adt3DExtentTests.cpp
        test/Adt3DPointTests.cpp
        test/AdtTests.cpp
        test/AflrToCGNSTests.cpp
        test/BlockSparseMatrixTests.cpp
        test/BuildCRSTests.cpp
        test/CartBlockSliceTests.cpp
        test/CartBlockTests.cpp
        test/CGNSFaceExtractionTests.cpp
        test/CGNSToAflrTests.cpp
        test/CGNSToFun3dTests.cpp
        test/CGNSToVtkTests.cpp
        test/DualMetricsTests.cpp
        test/ExtentMeshWrapperTests.cpp
        test/ExtentBuilderTests.cpp
        test/ExtentWriterTests.cpp
        test/EdgesTests.cpp
        test/EdgeFieldLeastSquaresGradientTests.cpp
        test/ExtentTests.cpp
        test/GreedyGraphColoringTests.cpp
        test/FacetTests.cpp
        test/Fun3DToCGNSTests.cpp
        test/LinearPartitionerTests.cpp
        test/SparseMatrixTests.cpp
        test/MeshInterfaceTests.cpp
        test/NormalTests.cpp
        test/PrintLargeIntegerTests.cpp
        test/ParfaitMeshTests.cpp
        test/DistanceTreeTests.cpp
        test/PointTest.cpp
        test/StencilLeastSquaresGradientTests.cpp
        test/StringToolsTests.cpp
        test/STLTests.cpp
        test/VectorToolsTests.cpp
        test/MinimumStorageSetTests.cpp
        test/ImportedUgridTests.cpp
        test/OctreeTests.cpp
        test/NodeToCellTests.cpp
        test/HexMetricsTests.cpp
        test/GraphOrderingsTests.cpp
        test/PyramidMetricsTests.cpp
        test/PrismMetricsTests.cpp
        test/TetMetricsTests.cpp
        test/NodeToNodeBuilderTests.cpp
        test/DistanceTreeTests.cpp
        test/DistanceCalculatorTests.cpp
        test/ParallelExtent_test.cpp
        test/SyncFieldTests.cpp
        test/SyncPatternTests.cpp
        )
    add_executable(ParfaitTests ${SERIAL_TESTS_SOURCES} ${PARFAIT_HEADERS})
    target_include_directories(ParfaitTests
            PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:/.>
        )
    target_link_libraries(ParfaitTests parfait MessagePasser::MessagePasser Catch2::Catch)
    target_compile_definitions(ParfaitTests INTERFACE ${PARFAIT_TEST_CXX_FLAGS})

endif()

add_library(parfait INTERFACE)
add_library(parfait::parfait ALIAS parfait)
target_include_directories(parfait INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
    ${MPI_INCLUDE_PATH}
    )

target_link_libraries(parfait INTERFACE ${MPI_LIBRARIES}
    MessagePasser::MessagePasser
    nlohmann_json
    )

install(TARGETS parfait
    EXPORT parfait
    LIBRARY DESTINATION lib)

install(FILES ${PARFAIT_HEADERS} DESTINATION include/parfait)

install(EXPORT parfait
    NAMESPACE parfait:: DESTINATION cmake)

install(FILES parfaitConfig.cmake
    DESTINATION cmake)

enable_testing()
set (ranks_1 -np 1 "./ParfaitTests")
set (ranks_2 -np 2 "./ParfaitTests")
add_test(NAME parfait-1-rank COMMAND "mpirun" ${ranks_1})
add_test(NAME parfait-2-rank COMMAND "mpirun" ${ranks_2})
