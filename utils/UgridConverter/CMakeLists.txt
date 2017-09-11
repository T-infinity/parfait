cmake_minimum_required(VERSION 2.8)

PROJECT(UgridConverter)
set (CMAKE_CXX_STANDARD 14)

find_package(VTK REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(GridConverter UgridConverter.cpp)
target_link_libraries(GridConverter ${VTK_LIBRARIES})

target_include_directories(GridConverter PUBLIC
        ../../parfait/src/
        ../../MessagePasser/src/
        ../../parfait-viz/src/
        ${VTK_INCLUDE_DIRS}
        ${PARMETIS_INCLUDE_DIRS}
        ${METIS_INCLUDE_DIRS}
        ${MPI_INCLUDE_PATH}
        )
target_link_libraries(GridConverter
    nlohmann_json
    )


install(TARGETS GridConverter
    EXPORT parfait-targets
    DESTINATION parfait/bin)

add_subdirectory(test)
