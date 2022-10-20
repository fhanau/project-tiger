add_executable(client main.cpp parser.h parser.cpp requester.cpp requester.h)
target_link_libraries(client PRIVATE Crow::Crow curlpp)