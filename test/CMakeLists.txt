include_directories(${TIGER_SOURCE_DIR}/game)
include_directories(${TIGER_SOURCE_DIR}/sqliteDB)

add_subdirectory(googletest EXCLUDE_FROM_ALL)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(TESTS DatabaseTests.cpp serverApiTests.cpp sqliteTest.cpp
    cardTests.cpp deckTests.cpp statTests.cpp serverMockTests.cpp)
set(TEST_DEPS ${TIGER_SOURCE_DIR}/server/tiger.cpp ${TIGER_SOURCE_DIR}/server/tiger.h ${TIGER_SOURCE_DIR}/server/auth.cpp ${TIGER_SOURCE_DIR}/server/auth.h
    mockDatabase.h mockDatabase.cpp)
set(TEST_GAME_DEPS ${TIGER_SOURCE_DIR}/game/card.cpp
    ${TIGER_SOURCE_DIR}/game/card.h
    ${TIGER_SOURCE_DIR}/game/deck.h
    ${TIGER_SOURCE_DIR}/game/deck.cpp
)
set(TEST_DB_DEPS
    ${TIGER_SOURCE_DIR}/sqliteDB/sql.h
    ${TIGER_SOURCE_DIR}/sqliteDB/sql.cpp
    ${TIGER_SOURCE_DIR}/sqliteDB/stat.h
    ${TIGER_SOURCE_DIR}/sqliteDB/stat.cpp
    ${TIGER_SOURCE_DIR}/libraries/sqlite/sqlite3.c
    ${TIGER_SOURCE_DIR}/libraries/sqlite/sqlite3.h
)
# create the test executable
add_executable(tiger-test ${TESTS} ${TEST_DEPS} ${TEST_GAME_DEPS} ${TEST_DB_DEPS})
target_link_libraries(tiger-test PRIVATE Crow::Crow GTest::gtest_main GTest::gmock ${CMAKE_DL_LIBS})

#Run end-to-end tests. See the readme, end-end.sh and client-commands.txt for details
add_custom_target(end-end-tests
  DEPENDS client server
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND ${TIGER_TEST_DIR}/end-end.sh)

# setup gcovr
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
include(CodeCoverage)

message(STATUS "Coverage analysis with gcov enabled")
append_coverage_compiler_flags()

setup_target_for_coverage_gcovr_html(
    NAME coverage
    EXECUTABLE ./tiger-test
    DEPENDENCIES tiger-test
    BASE_DIRECTORY "${TIGER_SOURCE_DIR}"
    EXCLUDE "${TIGER_SOURCE_DIR}/libraries"
    EXCLUDE "${TIGER_SOURCE_DIR}/client"
)
