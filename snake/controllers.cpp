#include "controllers.hpp"
#include "snake.hpp"

namespace Game_logic {
    Player_controller::Player_controller(Arrow_key_catcher& keys) noexcept
        : Controller { }
        , keys_ { keys }
    { }
    
    void Player_controller::think(Snake& snake)
    {
        if (keys_.is_down(SDL_SCANCODE_UP))
        {
            snake.set_direction(Direction::up);
        }
        
        if (keys_.is_down(SDL_SCANCODE_DOWN))
        {
            snake.set_direction(Direction::down);
        }
        
        if (keys_.is_down(SDL_SCANCODE_LEFT))
        {
            snake.set_direction(Direction::left);
        }
        
        if (keys_.is_down(SDL_SCANCODE_RIGHT))
        {
            snake.set_direction(Direction::right);
        }
    }
    
    Controller_uptr Player_controller::clone() const
    {
        return std::make_unique<Player_controller>(keys_);
    }
}
