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
