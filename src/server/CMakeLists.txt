add_executable(server main.cpp util.cpp util.h)
target_link_libraries(server PRIVATE Crow::Crow)
