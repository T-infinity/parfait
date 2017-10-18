cmake_minimum_required(VERSION 3.6)

PROJECT(DistanceCalculator)
set (CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)

add_executable(DistanceCalculator DistanceCalculator.cpp)
target_link_libraries(DistanceCalculator PUBLIC parfait nlohmann_json)

install(TARGETS DistanceCalculator 
    EXPORT parfait-targets
    DESTINATION parfait/bin)
