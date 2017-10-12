cmake_minimum_required(VERSION 3.0)



set(TEST_SOURCES
        AllTests.cpp
        CommandLineParserTests.cpp
        )


add_executable(UnitTests ${TEST_SOURCES})
target_include_directories(UnitTests PUBLIC
        ${CATCH_INCLUDE_DIR}
        ${JSON_INCLUDE_DIR}
        )

target_link_libraries(UnitTests nlohmann_json)

add_dependencies(UnitTests catch)
