cmake_minimum_required(VERSION 3.6)

PROJECT(DistanceCalculator)
set (CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(VTK REQUIRED)

add_executable(AdtDistanceCalculator DistanceCalculator.cpp)
target_include_directories(AdtDistanceCalculator PUBLIC ${VTK_INCLUDE_DIRS})
target_link_libraries(AdtDistanceCalculator PUBLIC parfait nlohmann_json ${VTK_LIBRARIES} ${OpenMP_C_FLAGS} MessagePasser)
target_compile_options(AdtDistanceCalculator PUBLIC ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS})

install(TARGETS AdtDistanceCalculator
    EXPORT parfait-targets
    DESTINATION parfait/bin)


add_executable(DistanceCalculator TreeDistanceCalculator.cpp)
target_include_directories(DistanceCalculator PUBLIC ${VTK_INCLUDE_DIRS})
target_link_libraries(DistanceCalculator PUBLIC parfait nlohmann_json ${VTK_LIBRARIES} ${OpenMP_C_FLAGS} MessagePasser)
target_compile_options(DistanceCalculator PUBLIC ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS})

install(TARGETS DistanceCalculator
        EXPORT parfait-targets
        DESTINATION parfait/bin)


