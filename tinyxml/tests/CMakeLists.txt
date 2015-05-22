
set(test_SRC
        "xmltest.cpp"
    )

add_executable(tinyxml_tests ${test_SRC})
target_link_libraries(tinyxml_tests tinyxml)
