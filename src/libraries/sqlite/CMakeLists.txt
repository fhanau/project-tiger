#target_link_libraries(sql PRIVATE Threads::Threads)

#add_executable(sql shell.c sqlite3.c sqlite3.h sqlite3ext.h)
#find_package(Threads REQUIRED) 
#add_library(mysqlite3 sqlite3.c sqlite3.h sqlite3ext.h)
#target_link_libraries(mysqlite3 Threads::Threads ${CMAKE_DL_LIBS})
