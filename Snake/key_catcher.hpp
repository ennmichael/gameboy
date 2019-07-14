#pragma once

#include "Sdl-Plus-Plus/flow.h"
#include <cassert>
#include <unordered_map>
#include <optional>

namespace Game_logic {
class Key_catcher {
   public:
    bool is_down(SDL_Scancode) const noexcept;
    void update(const SDL_Event&) noexcept;

   private:
    bool catches(SDL_Scancode) const noexcept;
    void clear() noexcept;
    void set(SDL_Scancode) noexcept;

    std::unordered_map<SDL_Scancode, bool> keys_{
        {SDL_SCANCODE_LEFT, false},
        {SDL_SCANCODE_RIGHT, false},
        {SDL_SCANCODE_UP, false},
        {SDL_SCANCODE_DOWN, false},
    };
    std::optional<SDL_Scancode> last_{std::nullopt};
};
}
