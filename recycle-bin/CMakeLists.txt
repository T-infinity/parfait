cmake_minimum_required(VERSION 2.6)
project(parfait)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror -Wall")
link_directories(
    ${CMAKE_LIBRARY_PATH}
    )

include_directories(
  ${CMAKE_INCLUDE_PATH}
  core
  utils
	${PARMETIS_INCLUDE_PATH}
  )

add_definitions(
    -DOMPI_SKIP_MPICXX
    -DMPI_NO_CPPBIND
  )

option(PARFAIT_WITH_MPI "Configure with MPI" ON)
option(PARFAIT_WITH_PARMETIS "Configure with ParMetis" ON)
option(PARFAIT_WITH_VTK "Configure with ParMetis" ON)

set(CATCH_DIR "${CMAKE_SOURCE_DIR}/third_party_deps/catch")
set(TINY_XML_DIR "${CMAKE_SOURCE_DIR}/third_party_deps/tinyxml")

find_path ( PARMETIS_INCLUDE_PATH parmetis.h)
find_library ( PARMETIS_LIBRARY NAMES parmetis )
find_library ( METIS_LIBRARY NAMES metis )
include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args ( ParMetis DEFAULT_MSG PARMETIS_LIBRARY PARMETIS_INCLUDE_PATH )



add_subdirectory(utils)
add_subdirectory(core/c_interfaces)
add_subdirectory(plugins)
add_subdirectory(third_party_deps/tinyxml)

add_subdirectory(core/tests/serial_tests)

if(PARFAIT_WITH_MPI)
  add_subdirectory(core/tests/parallel_tests)
endif()

if(PARFAIT_WITH_VTK)
  add_subdirectory(core/tests/vtk_tests)
endif()
