add_executable(client main.cpp parser.h parser.cpp requester.cpp requester.h)
target_include_directories(client PRIVATE ../libraries/curlpp/include)
target_link_libraries(client PRIVATE Crow::Crow)
