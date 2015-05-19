
set(tinyxml_SRC
        "tinystr.cpp"  
        "tinyxml.cpp"  
        "tinyxmlerror.cpp"  
        "tinyxmlparser.cpp"  
        "xmltest.cpp")
include_directories(${TINYXML_CMAKE_DIR})
add_library(tinyxml ${tinyxml_SRC})
