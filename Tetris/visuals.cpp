#include "visuals.h"
#include "matrix.h"
#include <iostream>

namespace Visuals {
void draw_block(Sdl::Screen& screen, Sdl::Point pos, SDL_Color color) {
    pos *= Sdl::Point{block_width, block_height};

    screen.add_draw(Sdl::make_rect(pos, block_width, block_height), color,
                    Sdl::Color_filling::Filled);
}

void draw_game_element(Sdl::Screen& screen,
                       const Game_logic::Tetromino& tetromino) {
    for (const auto& t_block_pos : Game_logic::block_positions(tetromino)) {
        draw_block(screen, t_block_pos, tetromino.color_);
    }
}

void draw_game_element(Sdl::Screen& screen,
                       const Game_logic::Tetromino_table& table) {
    Util::for_each(table.blocks(), [&screen](auto pos, const auto& e) {
        if (e)
            draw_block(screen, pos, *e);
    });
}
}
