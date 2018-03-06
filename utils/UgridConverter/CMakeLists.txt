cmake_minimum_required(VERSION 3.6)

PROJECT(UgridConverter)
set (CMAKE_CXX_STANDARD 14)

find_package(VTK)

if(VTK_FOUND)
    add_executable(GridConverter UgridConverter.cpp CommandLineParser.h)
    target_link_libraries(GridConverter ${VTK_LIBRARIES})

    target_include_directories(GridConverter PUBLIC ../../parfait-viz/src/ ${VTK_INCLUDE_DIRS} )
    target_link_libraries(GridConverter nlohmann_json parfait MessagePasser)

    install(TARGETS GridConverter
            EXPORT parfait-targets
            DESTINATION parfait/bin)

    if(BUILD_TESTS)
        add_subdirectory(test)
    endif()

endif()
