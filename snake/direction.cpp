#include "direction.hpp"

namespace Game_logic {
Sdl::Point direction_vector(Direction dir) noexcept {
    switch (dir) {
        case Direction::up:
            return {0, -1};
        case Direction::down:
            return {0, 1};
        case Direction::left:
            return {-1, 0};
        case Direction::right:
            return {1, 0};
    }

    return {};
}
}  // End namespace Game_logic
