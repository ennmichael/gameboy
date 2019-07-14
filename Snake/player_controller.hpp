#pragma once

#include "controller.hpp"
#include "key_catcher.hpp"

namespace Game_logic {
// Player_controller:
// a controller for the player snake.
// control() behaves in regard to key presses.
class Player_controller : public Controller {
   public:
    explicit Player_controller(const Key_catcher&) noexcept;

    void control(Snake& self) override;

   private:
    const Key_catcher* keys_;
};
}
