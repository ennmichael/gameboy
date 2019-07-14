#ifndef _DIRECTION_HPP_
#define _DIRECTION_HPP_

#include "sdl-plus-plus/drawing.h"

namespace Game_logic {
enum class Direction { up, down, left, right };

Sdl::Point direction_vector(Direction) noexcept;
}

#endif
