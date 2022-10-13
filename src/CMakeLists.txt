cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
project (TIGER VERSION 0.1)
set (CMAKE_CXX_STANDARD 17)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused -Wno-unused-parameter -Wno-pessimizing-move")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused -Wno-unused-parameter -Wno-pessimizing-move")
endif()

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(test)
add_subdirectory(libraries/Crow)
add_subdirectory(libraries/sqlite)

add_custom_target(code-tests)
add_dependencies(code-tests code-spell code-lint code-analyze)
#target for codespell, exit 0 is to work around nonzero return code
add_custom_target(code-spell
                  COMMAND codespell ${CMAKE_SOURCE_DIR}/CMakeLists.txt  ${CMAKE_SOURCE_DIR}/server ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/test ${CMAKE_SOURCE_DIR}/../README.md || (exit 0))
add_custom_target(code-lint
                  COMMAND cpplint --recursive ${CMAKE_SOURCE_DIR}/server ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/test || (exit 0))
add_custom_target(code-analyze
                  COMMAND cppcheck --enable=warning,style,unusedFunction ${CMAKE_SOURCE_DIR}/server ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/test)
