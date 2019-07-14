cmake_minimum_required(VERSION 3.1.0)
project(gameboy)

macro(add_warnings target)
if(MSVC)
	target_compile_options(${target} PRIVATE /W4 /WX)
else()
	target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
endif()
endmacro()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${gameboy_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

set(SDL2_PATH "${gameboy_SOURCE_DIR}/SDL2-2.0.9")
find_package(SDL2 REQUIRED)

add_library(sdl_plus_plus Sdl-Plus-Plus/drawing.cpp Sdl-Plus-Plus/flow.cpp Sdl-Plus-Plus/resource.cpp)
add_warnings(sdl_plus_plus)

if (SDL2_FOUND)
target_include_directories(sdl_plus_plus PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(sdl_plus_plus PRIVATE ${SDL2_LIBRARY})
else()
message(FATAL_ERROR "SDL2 not found. If you're on Linux, try running sudo apt-get install libsdl2-dev. On Windows, download the development libraries from http://www.libsdl.org/download-2.0.php")
endif()

add_executable(Snake Snake/direction.cpp Snake/food.cpp Snake/game.cpp Snake/main.cpp Snake/player_controller.cpp Snake/snake.cpp Snake/visuals.cpp Snake/ai_controller.cpp Snake/key_catcher.cpp)
target_include_directories(Snake PRIVATE ${gameboy_SOURCE_DIR})
target_link_libraries(Snake sdl_plus_plus)
add_warnings(Snake)

add_executable(Tetris Tetris/game.cpp Tetris/game_logic.cpp Tetris/main.cpp Tetris/visuals.cpp)
target_include_directories(Tetris PRIVATE ${gameboy_SOURCE_DIR})
target_link_libraries(Tetris sdl_plus_plus)
add_warnings(Tetris)
