cmake_minimum_required(VERSION 3.21)
project(ncursese_game_c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-lncurses")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(main main.c)
add_executable(client client.c)
add_executable(test test.c)

target_link_libraries(client ${CURSES_LIBRARIES})
target_link_libraries(main ${CURSES_LIBRARIES})

target_link_libraries(test ${CURSES_LIBRARIES})

TARGET_LINK_LIBRARIES(test pthread)