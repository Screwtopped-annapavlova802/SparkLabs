include(GoogleTest)

add_executable(tests 
    test_main.cpp
)
target_link_libraries(tests 
    GTest::gtest_main
    core
    engine
    ai
    render
    platform
)
gtest_discover_tests(tests)
