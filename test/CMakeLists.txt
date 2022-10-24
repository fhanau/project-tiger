add_subdirectory(googletest)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(tiger-test DatabaseTests.cpp serverApiTests.cpp ../src/server/util.h ../src/server/util.cpp ../src/sqliteDB/sql.h ../src/sqliteDB/sql.cpp ../src/libraries/sqlite/sqlite3.c ../src/libraries/sqlite/sqlite3.h)

target_link_libraries(tiger-test PRIVATE Crow::Crow)
target_link_libraries(tiger-test PRIVATE GTest::gtest_main)
target_link_libraries(tiger-test PRIVATE GTest::gmock)
