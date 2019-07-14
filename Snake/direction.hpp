#pragma once

#include "Sdl-Plus-Plus/drawing.h"

namespace Game_logic {
enum class Direction { up, down, left, right };

Sdl::Point direction_vector(Direction) noexcept;
}
