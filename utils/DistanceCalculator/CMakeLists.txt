cmake_minimum_required(VERSION 3.6)

PROJECT(DistanceCalculator)
set (CMAKE_CXX_STANDARD 14)

find_package(OpenMP)
if(NOT OpenMP_FOUND)
    message("======================================")
    message("OpenMP was not detected on your system.")
    message("The DistanceCalculator Utility will run slower as a result")
    message("This error often occurs on Apple hardware since clang on Apple does not ship with OpenMP support")
    message("We suggest using GNU g++ even on Apple hardware for this reason.")
    message("You can set the compiler when invoking cmake with: ")
    message("   cmake -DCMAKE_CXX_COMPILER=/path/to/your/desired/C++/compiler -DCMAKE_C_COMPILER=/path/to/your/desired/C/compiler"
    message("======================================")
endif()


add_executable(DistanceCalculator TreeDistanceCalculator.cpp)
target_include_directories(DistanceCalculator PUBLIC ${VTK_INCLUDE_DIRS})
target_link_libraries(DistanceCalculator PUBLIC parfait ${OpenMP_C_FLAGS} MessagePasser)
target_compile_options(DistanceCalculator PUBLIC ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS})
install(TARGETS DistanceCalculator
        EXPORT parfait-targets
        DESTINATION parfait/bin)


