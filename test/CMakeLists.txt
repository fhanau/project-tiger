add_subdirectory(googletest)
add_executable(tiger-test DatabaseTests.cpp serverApiTests.cpp ../src/server/util.h ../src/sqliteDB/sql.h)

target_link_libraries(tiger-test PRIVATE Crow::Crow)
target_link_libraries(tiger-test PRIVATE GTest::gtest_main)
target_link_libraries(tiger-test PRIVATE GTest::gmock)
