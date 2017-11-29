cmake_minimum_required(VERSION 3.6)

PROJECT(STLHelp)
set (CMAKE_CXX_STANDARD 14)

add_executable(STLHelp STLHelp.cpp)

target_link_libraries(STLHelp parfait MessagePasser)

install(TARGETS STLHelp
    EXPORT parfait-targets
    DESTINATION parfait/bin)
