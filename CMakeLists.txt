cmake_minimum_required(VERSION 3.1.0)
project(gameboy)

macro(add_warnings target)
	if(MSVC)
		target_compile_options(${target} PRIVATE /W4 /WX)
	else()
		target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic -Werror)
	endif()
endmacro()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${gameboy_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

set(SDL2_PATH "${gameboy_SOURCE_DIR}/SDL2-2.0.9")
find_package(SDL2 REQUIRED)

add_library(sdl_plus_plus sdl-plus-plus/drawing.cpp sdl-plus-plus/flow.cpp sdl-plus-plus/main.cpp sdl-plus-plus/resource.cpp)
add_warnings(sdl_plus_plus)

if (SDL2_FOUND)
        target_include_directories(sdl_plus_plus PRIVATE ${SDL2_INCLUDE_DIR})
        target_link_libraries(sdl_plus_plus PRIVATE ${SDL2_LIBRARY})
else()
        message(FATAL_ERROR "SDL2 not found. If you're on Linux, try running sudo apt-get install libsdl2-dev. On Windows, download the development libraries from http://www.libsdl.org/download-2.0.php")
endif()

add_executable(snake snake/ai_controller.cpp snake/controllers.cpp snake/direction.cpp snake/food.cpp snake/game.cpp snake/main.cpp snake/player_controller.cpp snake/snake.cpp snake/visuals.cpp)
target_include_directories(snake PRIVATE ${gameboy_SOURCE_DIR})
target_link_libraries(snake sdl_plus_plus)
add_warnings(snake)

add_executable(tetris tetris/game.cpp tetris/game_logic.cpp tetris/main.cpp tetris/visuals.cpp)
target_include_directories(tetris PRIVATE ${gameboy_SOURCE_DIR})
target_link_libraries(tetris sdl_plus_plus)
add_warnings(tetris)
