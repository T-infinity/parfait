cmake_minimum_required(VERSION 2.8)

PROJECT(UgridConverter)
set (CMAKE_CXX_STANDARD 14)

find_package(VTK REQUIRED)

include_directories(
        ../../parfait/src/
        ../../MessagePasser/src/
        ../../parfait-viz/src/
    ${VTK_INCLUDE_DIRS}
        ${PARMETIS_INCLUDE_DIRS}
        ${METIS_INCLUDE_DIRS}
        ${MPI_INCLUDE_PATH}
        ${JSON_INCLUDE_DIR}
)

add_executable(UgridConverter UgridConverter.cpp)
target_link_libraries(UgridConverter ${VTK_LIBRARIES})

install(TARGETS UgridConverter 
    EXPORT parfait-targets
    DESTINATION parfait/bin)
