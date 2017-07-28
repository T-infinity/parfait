cmake_minimum_required(VERSION 3.0)

PROJECT(Parfait)
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#--------------- Add catch as external project----------------------------------------------------------------
include(ExternalProject)
find_package(Git REQUIRED)


ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch source_dir)

set(CATCH_INCLUDE_DIR ${source_dir}/single_include CACHE INTERNAL "Path to include folder for Catch")
#---------------- End adding catch as external project -------------------------------------------------------

add_subdirectory(parfait)
add_subdirectory(parfait-viz)
add_subdirectory(utils)
add_subdirectory(MessagePasser)

