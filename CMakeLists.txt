cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
project (TIGER VERSION 0.1)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused -Wno-unused-parameter -Wno-pessimizing-move -Wno-missing-field-initializers")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused -Wno-unused-parameter -Wno-missing-field-initializers")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

set(TIGER_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(TIGER_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
add_subdirectory(${TIGER_SOURCE_DIR}/server)
add_subdirectory(${TIGER_SOURCE_DIR}/client)
set(CROW_FEATURES ssl)
add_subdirectory(${TIGER_SOURCE_DIR}/libraries/Crow)
set(CURLPP_BUILD_SHARED_LIBS OFF)
add_subdirectory(${TIGER_SOURCE_DIR}/libraries/curlpp EXCLUDE_FROM_ALL)
include_directories(${TIGER_SOURCE_DIR}/libraries/sqlite)

#Import tests so they can be built from the top level.
add_subdirectory(test)

add_custom_target(all-tests DEPENDS code-tests run-tiger-test)
add_custom_target(run-tiger-test
  DEPENDS tiger-test
  COMMAND tiger-test || (exit 0))

add_custom_target(code-tests)
add_dependencies(code-tests code-spell code-lint code-analyze)
#target for codespell, exit 0 is to work around nonzero return code
set(TIGER_CODE_FILES server/ client/ ../test/*.cpp game/ sqliteDB/)
set(TIGER_TEXT_FILES CMakeLists.txt ../README.md ../test/CMakeLists.txt ../.github/ ../test/client-commands.sh)
add_custom_target(code-spell
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  COMMAND codespell ${TIGER_CODE_FILES} ${TIGER_TEXT_FILES} || (exit 0))
add_custom_target(code-lint
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  COMMAND cpplint --recursive --filter=-legal/copyright,-build/include_subdir,-build/header_guard,-runtime/explicit,-readability/todo,-readability/casting,-whitespace/comments ${TIGER_CODE_FILES} || (exit 0))
add_custom_target(code-analyze
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  #make sure to not analyze library headers, avoids warnings that don't apply to us and much faster
  COMMAND cppcheck --enable=warning,style --std=c++17 --suppress=noExplicitConstructor --suppress=cstyleCast ${TIGER_CODE_FILES})

#tests for CI, at most 20 linting/static analyzer issues
add_custom_target(code-lint-ci
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  COMMAND test `cpplint --recursive --filter=-legal/copyright,-build/include_subdir,-build/header_guard,-runtime/explicit,-readability/todo,-readability/casting,-whitespace/comments --quiet --output=sed ${TIGER_CODE_FILES} 2>&1 >/dev/null | wc -l` -le 20)
add_custom_target(code-spell-ci
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  COMMAND codespell ${TIGER_CODE_FILES} ${TIGER_TEXT_FILES})
add_custom_target(code-analyze-ci
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  COMMAND test `cppcheck --enable=warning,style --std=c++17 --suppress=noExplicitConstructor --suppress=cstyleCast --template='{line}' -q ${TIGER_CODE_FILES} 2>&1 >/dev/null | wc -l` -le 20)
