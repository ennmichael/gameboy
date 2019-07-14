#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include "sdl-plus-plus/drawing.h"

namespace Constants {
constexpr auto game_size = 14;
constexpr auto rect_size = 20;
constexpr auto window_size = game_size * rect_size;

constexpr auto snake_color = Sdl::color_black();
constexpr auto food_color = Sdl::color_red();
constexpr auto background_color = Sdl::color_white();
};

#endif
