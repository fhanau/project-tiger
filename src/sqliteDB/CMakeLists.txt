#add_executable(sqliteDB main.cpp) 
#target_link_libraries(sqliteDB PRIVATE sqlite::sqlite)

add_library(sqlite3 STATIC ../libraries/sqlite/sqlite3.c ../libraries/sqlite/sqlite3.h ../libraries/sqlite/sqlite3ext.h ../libraries/sqlite/shell.c)
target_include_directories(my_lib PUBLIC ./)

add_executable(sqliteDB main.cpp sql.h sql.cpp)
target_link_libraries(sqliteDB PRIVATE sqlite3)