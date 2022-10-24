include_directories(${CMAKE_SOURCE_DIR}/game/)

add_subdirectory(googletest)

add_executable(
    tiger-test 
    DatabaseTests.cpp 
    serverApiTests.cpp
    cardTests.cpp
    ${CMAKE_SOURCE_DIR}/game/card.h
    ${CMAKE_SOURCE_DIR}/game/card.cpp
    ${CMAKE_SOURCE_DIR}/game/deck.h
    ${CMAKE_SOURCE_DIR}/game/deck.cpp
)


target_link_libraries(tiger-test PRIVATE Crow::Crow)
target_link_libraries(tiger-test PRIVATE GTest::gtest_main)
target_link_libraries(tiger-test PRIVATE GTest::gmock)