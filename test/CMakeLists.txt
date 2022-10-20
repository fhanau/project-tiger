add_subdirectory(googletest)
add_executable(tiger-test DatabaseTests.cpp serverApiTests.cpp)

target_link_libraries(tiger-test PRIVATE Crow::Crow)
target_link_libraries(tiger-test PRIVATE GTest::gtest_main)
target_link_libraries(tiger-test PRIVATE GTest::gmock)
