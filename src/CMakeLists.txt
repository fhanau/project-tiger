cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
project (TIGER VERSION 0.1)
set (CMAKE_CXX_STANDARD 17)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused -Wno-unused-parameter -Wno-pessimizing-move")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused -Wno-unused-parameter -Wno-pessimizing-move")
endif()

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(libraries/Crow)

#While tests are done elsewhere, import them here since they also need the settings above.
#TODO: Consider moving this CMakeLists to the top level, should be cleaner since it sets up defines shared between src and test and both are isolated in different folders.
add_subdirectory(../test tests)

add_custom_target(all-tests)
add_dependencies(all-tests code-tests run-tiger-test)
add_custom_target(run-tiger-test
  DEPENDS tiger-test
  COMMAND tests/tiger-test || (exit 0))

add_custom_target(code-tests)
add_dependencies(code-tests code-spell code-lint code-analyze)
#target for codespell, exit 0 is to work around nonzero return code
set(TIGER_CODE_FILES ${CMAKE_SOURCE_DIR}/server ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/../test/*.cpp)
set(TIGER_TEXT_FILES ${CMAKE_SOURCE_DIR}/CMakeLists.txt ${CMAKE_SOURCE_DIR}/../README.md ${CMAKE_SOURCE_DIR}/../test/CMakeLists.txt)
add_custom_target(code-spell
  COMMAND codespell ${TIGER_CODE_FILES} ${TIGER_TEXT_FILES} || (exit 0))
add_custom_target(code-lint
  COMMAND cpplint --recursive --filter=-legal/copyright ${TIGER_CODE_FILES} || (exit 0))
add_custom_target(code-analyze
  COMMAND cppcheck --enable=warning,style,unusedFunction ${TIGER_CODE_FILES})
