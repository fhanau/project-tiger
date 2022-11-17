include_directories(${TIGER_SOURCE_DIR}/game)
include_directories(${TIGER_SOURCE_DIR}/sqliteDB)

add_subdirectory(googletest EXCLUDE_FROM_ALL)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(TESTS DatabaseTests.cpp serverApiTests.cpp sqliteTest.cpp cardTests.cpp deckTests.cpp statTests.cpp)
set(TEST_DEPS ${TIGER_SOURCE_DIR}/server/util.h ${TIGER_SOURCE_DIR}/server/util.cpp)
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

#This is just a mockup of how the client would be used for testing. To actually use the client in automated testing, we'd want to have it operate while the server is running, run the server with code coverage so we can get better coverage of the server frontend, and capture the client output to compare it with a file containing expected output.
add_custom_target(client-based-tests
  DEPENDS client
  #This command will freeze if the server is not running.
  COMMAND cat ${TIGER_TEST_DIR}/client-commands.sh | ${CMAKE_BINARY_DIR}/client)

# setup gcovr
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR/cmake})

message(STATUS "Coverage analysis with gcov enabled")    
include(CodeCoverage.cmake)
append_coverage_compiler_flags()
# TODO: change the local path
set(GCOVR_PATH "/opt/anaconda3/bin/gcovr")

# SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

setup_target_for_coverage_gcovr_html(
    NAME coverage
    EXECUTABLE ./tiger-test
    DEPENDENCIES tiger-test
)
