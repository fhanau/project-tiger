find_package(asio CONFIG REQUIRED)
target_link_libraries(main PRIVATE asio asio::asio)

find_package(Crow CONFIG REQUIRED)
target_link_libraries(main PRIVATE Crow::Crow)