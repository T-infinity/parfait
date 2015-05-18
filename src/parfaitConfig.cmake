# Compute paths to parfait (should work on different machines, etc)
get_filename_component(PARFAIT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(PARFAIT_INCLUDE_DIRS "@CONF_INCLUDE_DIRS@")

# cmake magic that Cameron doesn't understand, but is somehow necessary
if(NOT TARGET parfait AND NOT Parfait_BINARY_DIR)
    include("${PARFAIT_CMAKE_DIR}/ParfaitTargets.cmake")
endif()

# create the actual library ``parfait''
set(PARFAIT_LIBRARIES parfait)
