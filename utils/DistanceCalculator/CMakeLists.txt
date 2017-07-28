cmake_minimum_required(VERSION 3.0)

PROJECT(DistanceCalculator)
set (CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)

include_directories(
        ../../parfait/src
        ../../MessagePasser/src
        ${PARMETIS_INCLUDE_DIRS}
        ${METIS_INCLUDE_DIRS}
        ${MPI_INCLUDE_PATH}
        ${JSON_INCLUDE_DIR}
)

add_executable(DistanceCalculator DistanceCalculator.cpp)

install(TARGETS DistanceCalculator 
    EXPORT parfait-targets
    DESTINATION parfait/bin)
