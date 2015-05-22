
set(tinyxml_SRC
        "tinystr.cpp"  
        "tinyxml.cpp"  
        "tinyxmlerror.cpp"  
        "tinyxmlparser.cpp"  
        )
set(test_SRC
        "xmltest.cpp"
    )
include_directories(${TINYXML_CMAKE_DIR})
add_library(tinyxml ${tinyxml_SRC})

add_executable(tinyxml_tests ${test_SRC})
target_link_libraries(tinyxml_tests tinyxml)
