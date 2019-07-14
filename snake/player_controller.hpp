#ifndef _PLAYER_CONTROLLER_HPP_
#define _PLAYER_CONTROLLER_HPP_

#include "controller.hpp"
#include "key_catcher.hpp"

namespace Game_logic {
// Player_controller:
// a controller for the player snake.
// think() behaves in regard to key presses.
class Player_controller : public Controller {
   public:
    explicit Player_controller(const Technical::Arrow_key_catcher&) noexcept;

    void think(Snake& self) override;
    //     Controller_uptr clone() const override;

   private:
    const Technical::Arrow_key_catcher& keys_;
};
}

#endif
