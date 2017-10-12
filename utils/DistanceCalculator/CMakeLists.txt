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
)

add_executable(DistanceCalculator DistanceCalculator.cpp)
target_link_libraries(DistanceCalculator nlohmann_json)

install(TARGETS DistanceCalculator 
    EXPORT parfait-targets
    DESTINATION parfait/bin)
