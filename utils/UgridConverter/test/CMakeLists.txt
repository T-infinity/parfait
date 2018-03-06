cmake_minimum_required(VERSION 3.6)
if(BUILD_TESTS)

    set(TEST_SOURCES
            AllTests.cpp
            CommandLineParserTests.cpp
            )

    add_executable(GridConverterUnitTests ${TEST_SOURCES})
    target_include_directories(GridConverterUnitTests PUBLIC
            nlohmann_json
            )

    target_link_libraries(GridConverterUnitTests nlohmann_json Catch2::Catch)
endif()
