target_link_libraries(sql PRIVATE Threads::Threads)

add_executable(sql shell.c sqlite3.c sqlite3.h sqlite3ext.h)