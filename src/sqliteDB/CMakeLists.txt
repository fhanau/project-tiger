#add_executable(sqliteDB main.cpp) 
#target_link_libraries(sqliteDB PRIVATE sqlite::sqlite)

add_library(my_lib STATIC ../libraries/sqlite/sqlite3.c)
target_include_directories(my_lib PUBLIC ./)

add_executable(sqliteDB main.cpp sql.h sql.cpp)
target_link_libraries(sqliteDB PRIVATE my_lib)