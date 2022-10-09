cmake_minimum_required (VERSION 2.8.11)
project (TIGER)

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(test)

find_package(asio CONFIG REQUIRED)
target_link_libraries(main PRIVATE asio asio::asio)

find_package(Crow CONFIG REQUIRED)
target_link_libraries(main PRIVATE Crow::Crow)