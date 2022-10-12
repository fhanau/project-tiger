add_library(helper)
add_executable(client main.cpp helper/parser.h)
target_link_libraries(client PRIVATE Crow::Crow)
