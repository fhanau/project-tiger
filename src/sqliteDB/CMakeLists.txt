include_directories(${CMAKE_SOURCE_DIR}/libraries/sqlite/)
#add_executable(sqliteDB main.cpp) 
#target_link_libraries(sqliteDB PRIVATE sqlite::sqlite)

add_library(sqlite3 STATIC ../libraries/sqlite/sqlite3.c ../libraries/sqlite/sqlite3.h ../libraries/sqlite/sqlite3ext.h ../libraries/sqlite/shell.c)
target_include_directories(sqlite3 PUBLIC ./)


add_library(my_lib STATIC ../libraries/sqlite/sqlite3.c)
target_include_directories(my_lib PUBLIC ./)

#add_executable(sqliteDB main.cpp sql.cpp sql.h)
#target_link_libraries(sqliteDB PRIVATE my_lib)

#add_executable(sqliteDB main.cpp sql.h sql.cpp)
#target_link_libraries(sqliteDB PUBLIC ${CMAKE_DL_LIBS} mysqlite3)



