add_subdirectory(googletest)
add_executable(tiger-test DatabaseTests.cpp serverApiTests.cpp ../src/server/util.h ../src/server/util.cpp ../src/sqliteDB/sql.h ../src/sqliteDB/sql.cpp ../libraries/sqlite/sqlite3.c ../libraries/sqlite/sqlite3.h)

target_link_libraries(tiger-test PRIVATE Crow::Crow)
target_link_libraries(tiger-test PRIVATE GTest::gtest_main)
target_link_libraries(tiger-test PRIVATE GTest::gmock)
