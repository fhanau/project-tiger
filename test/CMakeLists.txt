include_directories(${TIGER_SOURCE_DIR}/game)
include_directories(${TIGER_SOURCE_DIR}/sqliteDB)

add_subdirectory(googletest EXCLUDE_FROM_ALL)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(TESTS DatabaseTests.cpp serverApiTests.cpp sqliteTest.cpp cardTests.cpp
    deckTests.cpp statTests.cpp integrationTests.cpp serverMockTests.cpp
    authTests.cpp statMocks.cpp)
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
set(TEST_SSL_DEPS
    ${CMAKE_BINARY_DIR}/key.pem
    ${CMAKE_BINARY_DIR}/cert.pem
)

# create the test executable
add_executable(tiger-test ${TESTS} ${TEST_DEPS} ${TEST_GAME_DEPS} ${TEST_DB_DEPS} ${TEST_SSL_DEPS})
target_link_libraries(tiger-test PRIVATE Crow::Crow GTest::gtest_main GTest::gmock ${CMAKE_DL_LIBS})

#Create key and certificate for HTTPS. Due to cmake being a terrible build system, it is necessary to copy it here.
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/key.pem ${CMAKE_BINARY_DIR}/cert.pem
  COMMAND openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -subj '/O=CS 4156 Project Tiger/C=US/ST=New York/L=New York/CN=project.tiger' -keyout key.pem -out cert.pem
  COMMAND touch ${CMAKE_BINARY_DIR}/key.pem ${CMAKE_BINARY_DIR}/cert.pem
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

#Run end-to-end tests. See the readme, end-end.sh and client-commands.txt for details
add_custom_target(end-end-tests
  DEPENDS client server
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND $ENV{SHELL} -e ${TIGER_TEST_DIR}/end-end/end-end.sh)

# setup gcovr
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
include(CodeCoverage)

message(STATUS "Coverage analysis with gcov enabled")
append_coverage_compiler_flags()
set(GCOVR_ADDITIONAL_ARGS --exclude-unreachable-branches --decisions --txt)
setup_target_for_coverage_gcovr_html(
    NAME coverage
    EXECUTABLE ./tiger-test
    DEPENDENCIES tiger-test
    BASE_DIRECTORY "${TIGER_SOURCE_DIR}"
    EXCLUDE "${TIGER_SOURCE_DIR}/libraries"
    EXCLUDE "${TIGER_SOURCE_DIR}/client"
)
