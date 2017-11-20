cmake_minimum_required(VERSION 3.6)
find_package(nlohmann_json REQUIRED)

set(TEST_SOURCES
        AllTests.cpp
        CommandLineParserTests.cpp
        )


add_executable(UnitTests ${TEST_SOURCES})
target_include_directories(UnitTests PUBLIC
        ${CATCH_INCLUDE_DIR}
        nlohmann_json
        )

target_link_libraries(UnitTests nlohmann_json)

add_dependencies(UnitTests Catch)
