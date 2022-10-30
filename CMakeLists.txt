cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
project (TIGER VERSION 0.1)
set (CMAKE_CXX_STANDARD 17)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused -Wno-unused-parameter -Wno-pessimizing-move -Wno-missing-field-initializers")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused -Wno-unused-parameter -Wno-missing-field-initializers")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)

set(TIGER_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${TIGER_SOURCE_DIR}/server)
add_subdirectory(${TIGER_SOURCE_DIR}/client)
add_subdirectory(${TIGER_SOURCE_DIR}/libraries/Crow)
set(CURLPP_BUILD_SHARED_LIBS OFF)
add_subdirectory(${TIGER_SOURCE_DIR}/libraries/curlpp EXCLUDE_FROM_ALL)
add_subdirectory(${TIGER_SOURCE_DIR}/libraries/openssl-cmake EXCLUDE_FROM_ALL)

#While tests are done elsewhere, import them here since they also need the settings above.
#TODO: Consider moving this CMakeLists to the top level, should be cleaner since it sets up defines shared between src and test and both are isolated in different folders.
add_subdirectory(test)

add_custom_target(all-tests DEPENDS code-tests run-tiger-test)
add_custom_target(run-tiger-test
  DEPENDS tiger-test
  COMMAND test/tiger-test || (exit 0))

add_custom_target(code-tests)
add_dependencies(code-tests code-spell code-lint code-analyze)
#target for codespell, exit 0 is to work around nonzero return code
set(TIGER_CODE_FILES server client ../test/*.cpp game sqliteDB)
set(TIGER_TEXT_FILES CMakeLists.txt ../README.md ../test/CMakeLists.txt ../.github)
add_custom_target(code-spell
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  COMMAND codespell ${TIGER_CODE_FILES} ${TIGER_TEXT_FILES} || (exit 0))
add_custom_target(code-lint
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  COMMAND cpplint --recursive --filter=-legal/copyright,-build/include_subdir,-build/header_guard,-runtime/explicit,-readability/todo,-whitespace/comments ${TIGER_CODE_FILES} || (exit 0))
add_custom_target(code-analyze
  WORKING_DIRECTORY ${TIGER_SOURCE_DIR}
  COMMAND cppcheck --enable=warning,style,unusedFunction --std=c++17 --suppress=noExplicitConstructor --suppress=cstyleCast --suppress=uninitMemberVar --max-configs=4 ${TIGER_CODE_FILES})
