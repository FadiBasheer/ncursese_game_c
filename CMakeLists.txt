cmake_minimum_required(VERSION 3.21)
project(ncursese_game_c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-lncurses")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(ncursese_game_c
        main.c)
target_link_libraries(ncursese_game_c ${CURSES_LIBRARIES})