#pragma once

#include "game_logic.h"
#include "Sdl-Plus-Plus/drawing.h"
#include <cstddef>

/*
 * Code that has to do with drawing elements.
 */

namespace Visuals {
constexpr auto block_width = 20;
constexpr auto block_height = 20;

constexpr std::size_t window_width = block_width * Game_logic::table_width;
constexpr std::size_t window_height = block_height * Game_logic::table_height;

void draw_block(Sdl::Screen&, Sdl::Point, SDL_Color);
void draw_game_element(Sdl::Screen&, const Game_logic::Tetromino&);
void draw_game_element(Sdl::Screen&, const Game_logic::Tetromino_table&);
}

