include_directories(${TIGER_SOURCE_DIR}/libraries/Crow/include)
add_executable(server main.cpp tiger.cpp tiger.h auth.cpp auth.h ../sqliteDB/sql.h ../sqliteDB/sql.cpp ../libraries/sqlite/sqlite3.c ../libraries/sqlite/sqlite3.h ${CMAKE_BINARY_DIR}/key.pem ${CMAKE_BINARY_DIR}/cert.pem)
target_link_libraries(server PRIVATE Crow::Crow ${CMAKE_DL_LIBS})

#Create key and certificate for HTTPS
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/key.pem ${CMAKE_BINARY_DIR}/cert.pem
  COMMAND openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -subj '/O=CS 4156 Project Tiger/C=US/ST=New York/L=New York/CN=project.tiger' -keyout key.pem -out cert.pem
  COMMAND touch ${CMAKE_BINARY_DIR}/key.pem ${CMAKE_BINARY_DIR}/cert.pem
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

