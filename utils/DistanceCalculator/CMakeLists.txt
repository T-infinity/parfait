cmake_minimum_required(VERSION 3.6)

PROJECT(DistanceCalculator)
set (CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)
find_package(VTK)


add_executable(DistanceCalculator TreeDistanceCalculator.cpp)
target_include_directories(DistanceCalculator PUBLIC ${VTK_INCLUDE_DIRS})
target_link_libraries(DistanceCalculator PUBLIC parfait nlohmann_json ${OpenMP_C_FLAGS} MessagePasser)
target_compile_options(DistanceCalculator PUBLIC ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS})
if(VTK_FOUND)
    target_link_libraries(DistanceCalculator PUBLIC ${VTK_LIBRARIES})
endif()

install(TARGETS DistanceCalculator
        EXPORT parfait-targets
        DESTINATION parfait/bin)


