add_subdirectory(googletest)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(tiger-test DatabaseTests.cpp serverApiTests.cpp ../src/server/util.h ../src/server/util.cpp ../src/sqliteDB/sql.h ../src/sqliteDB/sql.cpp ${CMAKE_SOURCE_DIR}/libraries/sqlite/sqlite3.c ${CMAKE_SOURCE_DIR}/libraries/sqlite/sqlite3.h)

target_link_libraries(tiger-test PRIVATE Crow::Crow GTest::gtest_main GTest::gmock)
