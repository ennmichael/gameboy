#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "controllers.hpp"
#include "food.hpp"
#include "sdl-plus-plus/flow.h"
#include "snake.hpp"
#include "visuals.hpp"
#include "ai_controller.hpp"

namespace Game_logic {
// This class is likely to change a lot
bool eats(const Snake&, const Food&) noexcept;

class Game {
   public:
    explicit Game(std::chrono::milliseconds) noexcept;

    void frame_advance(Sdl::Screen&);
    void handle_event(const SDL_Event&) noexcept;

   private:
    void check_interaction();
    void check_dead() noexcept;
    void redraw(Sdl::Screen&);

    Technical::Arrow_key_catcher keys_{};
    Food food_{};
    Snake player_ = [this] {
        Rule_vector rv{};

        rv.push_back(std::make_unique<Self_impact_rule>());
        rv.push_back(std::make_unique<Food_distance_rule>());

        return Snake{std::unique_ptr<Controller>{
            std::make_unique<AI_controller>(food_, std::move(rv))}};
    }();
    Sdl::Timer timer_;
    bool died_{false};
};
}  // End namespace Game_logic.

#endif
