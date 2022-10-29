include_directories(${CMAKE_SOURCE_DIR}/game/)
include_directories(${CMAKE_SOURCE_DIR}/sqliteDB/)

add_subdirectory(googletest EXCLUDE_FROM_ALL)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(TESTS DatabaseTests.cpp serverApiTests.cpp sqliteTest.cpp cardTests.cpp deckTests.cpp statTests.cpp)
set(TEST_DEPS ${PROJECT_SOURCE_DIR}/server/util.h ${PROJECT_SOURCE_DIR}/server/util.cpp)
set(TEST_GAME_DEPS ${CMAKE_SOURCE_DIR}/game/card.cpp 
    ${CMAKE_SOURCE_DIR}/game/card.h
    ${CMAKE_SOURCE_DIR}/game/deck.h
    ${CMAKE_SOURCE_DIR}/game/deck.cpp
)
set(TEST_DB_DEPS 
    ${PROJECT_SOURCE_DIR}/sqliteDB/sql.h
    ${PROJECT_SOURCE_DIR}/sqliteDB/sql.cpp 
    ${PROJECT_SOURCE_DIR}/sqliteDB/stat.h
    ${PROJECT_SOURCE_DIR}/sqliteDB/stat.cpp
    ${PROJECT_SOURCE_DIR}/libraries/sqlite/sqlite3.c 
    ${PROJECT_SOURCE_DIR}/libraries/sqlite/sqlite3.h
)
add_executable(tiger-test ${TESTS} ${TEST_DEPS} ${TEST_GAME_DEPS} ${TEST_DB_DEPS})
target_link_libraries(tiger-test PRIVATE Crow::Crow GTest::gtest_main GTest::gmock ${CMAKE_DL_LIBS})
