cmake_minimum_required(VERSION 3.6)
find_package(nlohmann_json REQUIRED)

set(TEST_SOURCES
        AllTests.cpp
        CommandLineParserTests.cpp
        )


add_executable(GridConverterUnitTests ${TEST_SOURCES})
target_include_directories(GridConverterUnitTests PUBLIC
        ${CATCH_INCLUDE_DIR}
        nlohmann_json
        )

target_link_libraries(GridConverterUnitTests nlohmann_json)

add_dependencies(GridConverterUnitTests Catch)
