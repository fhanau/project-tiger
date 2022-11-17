add_executable(client main.cpp parser.h parser.cpp requester.cpp requester.h util.h util.cpp)
target_include_directories(client PRIVATE ${TIGER_SOURCE_DIR}/libraries/curlpp/include)
target_link_libraries(client PRIVATE curlpp_static)
