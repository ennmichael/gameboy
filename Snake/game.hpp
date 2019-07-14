#pragma once

#include "Sdl-Plus-Plus/flow.h"
#include "controllers.hpp"
#include "food.hpp"
#include "snake.hpp"
#include "visuals.hpp"
#include "ai_controller.hpp"

namespace Game_logic {
bool eats(const Snake&, const Food&) noexcept;


class Game {
   public:
    enum class Controlled_by {Player, AI};
    Game(Controlled_by, std::chrono::milliseconds);

    void frame_advance(Sdl::Screen&);
    void handle_event(const SDL_Event&) noexcept;

   private:
    void check_interaction();
    void check_dead() noexcept;
    void redraw(Sdl::Screen&);

    Key_catcher keys_{};
    Food food_{};
    Snake snake_{nullptr};
    Sdl::Timer timer_;
    bool died_{false};
};
}  // End namespace Game_logic.

