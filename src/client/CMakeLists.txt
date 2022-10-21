add_executable(client main.cpp parser.h parser.cpp requester.cpp requester.h util.h util.cpp)
target_link_libraries(client PRIVATE Crow::Crow curlpp)