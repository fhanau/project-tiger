include_directories(${CMAKE_SOURCE_DIR}/game/)

add_subdirectory(googletest)
set(TESTS DatabaseTests.cpp serverApiTests.cpp cardTests.cpp)
set(TEST_DEPS ${CMAKE_SOURCE_DIR}/game/card.cpp ${CMAKE_SOURCE_DIR}/game/card.h ${PROJECT_SOURCE_DIR}/sqliteDB/sql.cpp ${PROJECT_SOURCE_DIR}/libraries/sqlite/sqlite3.c ${PROJECT_SOURCE_DIR}/libraries/sqlite/sqlite3.h)
add_executable(tiger-test ${TESTS} ${TEST_DEPS})

add_executable(
    tiger-test 
    DatabaseTests.cpp 
    serverApiTests.cpp
    cardTests.cpp
    ${CMAKE_SOURCE_DIR}/game/card.h
    ${CMAKE_SOURCE_DIR}/game/card.cpp
    ${CMAKE_SOURCE_DIR}/game/deck.h
    ${CMAKE_SOURCE_DIR}/game/deck.cpp
)

target_link_libraries(tiger-test PRIVATE Crow::Crow GTest::gtest_main GTest::gmock)
