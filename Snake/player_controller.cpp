#include "player_controller.hpp"
#include "food.hpp"
#include "snake.hpp"

namespace Game_logic {
Player_controller::Player_controller(const Key_catcher& keys) noexcept
    : Controller{},
      keys_{&keys} {}

void Player_controller::control(Snake& snake) {
    if (keys_->is_down(SDL_SCANCODE_UP)) {
        snake.set_direction(Direction::up);
    }

    if (keys_->is_down(SDL_SCANCODE_DOWN)) {
        snake.set_direction(Direction::down);
    }

    if (keys_->is_down(SDL_SCANCODE_LEFT)) {
        snake.set_direction(Direction::left);
    }

    if (keys_->is_down(SDL_SCANCODE_RIGHT)) {
        snake.set_direction(Direction::right);
    }
}
}
