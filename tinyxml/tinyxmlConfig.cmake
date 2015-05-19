# Compute paths to tinyxml (should work on different machines, etc)
get_filename_component(TINYXML_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(TINYXML_INCLUDE_DIRS "@CONF_INCLUDE_DIRS@")

# cmake magic that Cameron doesn't understand, but is somehow necessary
if(NOT TARGET tinyxml AND NOT Tinyxml_BINARY_DIR)
    include("${TINYXML_CMAKE_DIR}/tinyxmlTargets.cmake")
endif()

# create the actual library ``tinyxml''
set(TINYXML_LIBRARIES tinyxml)
