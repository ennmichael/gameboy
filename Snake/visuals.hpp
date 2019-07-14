#ifndef _VISUALS_HPP_
#define _VISUALS_HPP_

#include "food.hpp"
#include "Sdl-Plus-Plus/drawing.h"
#include "snake.hpp"

namespace Visuals {
/*
 * get_screen:
 * generates a specific screen for this application.
 */
Sdl::Screen get_screen();

void draw(Sdl::Screen&, const Game_logic::Snake&);
void draw(Sdl::Screen&, const Game_logic::Food&);
}  // End namespace Visuals

#endif
