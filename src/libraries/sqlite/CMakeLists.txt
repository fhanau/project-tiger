#target_link_libraries(sql PRIVATE Threads::Threads)

add_executable(sql shell.c sqlite3.c sqlite3.h sqlite3ext.h)

find_package(Threads REQUIRED) 
target_link_libraries(sql Threads::Threads ${CMAKE_DL_LIBS})
