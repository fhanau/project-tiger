#add_executable(sqliteDB main.cpp) 
#target_link_libraries(sqliteDB PRIVATE sqlite::sqlite)

#add_library(sqlite3 STATIC ../libraries/sqlite/sqlite3.c ../libraries/sqlite/sqlite3.h ../libraries/sqlite/sqlite3ext.h ../libraries/sqlite/shell.c)
#target_include_directories(sqlite3 PUBLIC ./)

add_executable(sqliteDB main.cpp sql.h sql.cpp ../libraries/sqlite/sqlite3.c ../libraries/sqlite/sqlite3.h ../libraries/sqlite/sqlite3ext.h ../libraries/sqlite/shell.c)
target_link_libraries(sqliteDB PUBLIC ${CMAKE_DL_LIBS})