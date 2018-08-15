cmake_minimum_required(VERSION 3.6)

PROJECT(DistanceCalculator)
set (CMAKE_CXX_STANDARD 14)
find_package(OpenMP)

add_executable(DistanceCalculator TreeDistanceCalculator.cpp)
target_link_libraries(DistanceCalculator PUBLIC parfait MessagePasser)
if(NOT OpenMP_FOUND)
    message("======================================")
    message("OpenMP was not detected on your system.")
    message("The DistanceCalculator Utility will run slower as a result")
    message("This error often occurs on Apple hardware since clang on Apple does not ship with OpenMP support")
    message("We suggest using GNU g++ even on Apple hardware for this reason.")
    message("You can set the compiler when invoking cmake with: ")
    message("   cmake -DCMAKE_CXX_COMPILER=/path/to/your/desired/C++/compiler -DCMAKE_C_COMPILER=/path/to/your/desired/C/compiler")
    message("======================================")
else()
    target_link_libraries(DistanceCalculator PUBLIC ${OpenMP_C_FLAGS})
    target_compile_options(DistanceCalculator PUBLIC ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS})
endif()


install(TARGETS DistanceCalculator
        EXPORT parfait-targets
        DESTINATION parfait/bin)
