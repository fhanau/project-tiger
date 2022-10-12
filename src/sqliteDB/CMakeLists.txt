add_executable(sqliteDB main.cpp)
target_link_libraries(sqliteDB PRIVATE sqlite::sqlite)