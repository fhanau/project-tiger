target_include_directories(client PRIVATE ../libraries/curlpp/include)
add_executable(client main.cpp parser.h parser.cpp requester.cpp requester.h)