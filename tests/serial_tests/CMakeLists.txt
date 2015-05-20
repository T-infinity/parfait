
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitSerialTests)
set(CATCH_DIR "/ump/fldmd/home/cdruyorj/cdruyor_local/catch/include")

find_package(parfait REQUIRED)

include_directories(
    "${CATCH_DIR}"
    "${parfait_DIR}/src"
    )

set(PARFAIT_SERIAL_TESTS_SRC
        AllTests.cpp  
        vtk_slice_writer_tests.cpp
        extent/ExtentBuilderTests.cpp  
        extent/extent_test.cpp
        VtkFieldTests/VtkFieldTests.cpp
       )


add_executable(ParfaitSerialTests ${PARFAIT_SERIAL_TESTS_SRC})
target_link_libraries(ParfaitSerialTests parfait CppUTest)
