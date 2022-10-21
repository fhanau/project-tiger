add_executable(server main.cpp util.cpp util.h ../sqliteDB/sql.h ../sqliteDB/sql.cpp)
target_link_libraries(server PRIVATE Crow::Crow ${CMAKE_DL_LIBS} mysqlite3)
