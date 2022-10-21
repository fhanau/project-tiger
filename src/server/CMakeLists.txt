add_executable(server main.cpp util.cpp util.h ../sqliteDB/sql.h ../sqliteDB/sql.cpp ../libraries/sqlite/sqlite3.c ../libraries/sqlite/sqlite3.h ../libraries/sqlite/sqlite3ext.h ../libraries/sqlite/shell.c)
target_link_libraries(server PRIVATE Crow::Crow ${CMAKE_DL_LIBS})
