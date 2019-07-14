#include "visuals.hpp"
#include "constants.hpp"

#include <iostream>

namespace Visuals {
namespace {
void draw_rect_at_point(Sdl::Screen& screen,
                        Sdl::Point point,
                        SDL_Color color) {
    screen.add_draw(
        Sdl::make_rect(
            point * Sdl::Point{Constants::rect_size, Constants::rect_size},
            Constants::rect_size - 1, Constants::rect_size - 1),
        color, Sdl::Color_filling::Filled);
}
}

Sdl::Screen get_screen() {
    Sdl::Screen_properties props{};
    props.title = "Snake";
    props.width = props.height = Constants::window_size;

    return Sdl::Screen{props};
}

void draw(Sdl::Screen& screen, const Game_logic::Snake& snake) {
    for (const auto& point : snake.points()) {
        draw_rect_at_point(screen, point, Constants::snake_color);
    }
}

void draw(Sdl::Screen& screen, const Game_logic::Food& food) {
    draw_rect_at_point(screen, food.position(), Constants::food_color);
}
}
