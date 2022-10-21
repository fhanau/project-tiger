find_package(sqlite3 CONFIG REQUIRED)
add_executable(server main.cpp util.cpp util.h ../sqliteDB/sql.h ../sqliteDB/sql.cpp)
target_link_libraries(server PRIVATE Crow::Crow)
target_link_libraries(server PUBLIC sqlite3)
