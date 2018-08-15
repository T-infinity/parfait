cmake_minimum_required(VERSION 3.6)

PROJECT(DistanceCalculator)
set (CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)


add_executable(DistanceCalculator TreeDistanceCalculator.cpp)
target_include_directories(DistanceCalculator PUBLIC ${VTK_INCLUDE_DIRS})
target_link_libraries(DistanceCalculator PUBLIC parfait ${OpenMP_C_FLAGS} MessagePasser)
target_compile_options(DistanceCalculator PUBLIC ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS})
install(TARGETS DistanceCalculator
        EXPORT parfait-targets
        DESTINATION parfait/bin)


