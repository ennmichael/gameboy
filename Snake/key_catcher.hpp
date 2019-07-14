#pragma once

#include "Sdl-Plus-Plus/flow.h"
#include <cassert>
#include <unordered_map>
#include <optional>

namespace Technical {
/*
 * Key_catcher:
 * checks keys. This is a template because I want
 * objects that catch different keys to be of different
 * types. Passing a Key_catcher that catches arrow keys
 * to a functions that excepts to be catching wasd
 * should be caught at compile time.
 */
template <SDL_Scancode... scancodes>
class Key_catcher {
   public:
    bool is_down(SDL_Scancode) const noexcept;
    void update(const SDL_Event&) noexcept;

   private:
    bool catches(SDL_Scancode) const noexcept;
    void clear() noexcept;
    void set(SDL_Scancode) noexcept;

    std::unordered_map<SDL_Scancode, bool> keys_{{scancodes, false}...};
    std::optional<SDL_Scancode> last_{std::nullopt};
};

template <SDL_Scancode... scancodes>
bool Key_catcher<scancodes...>::is_down(SDL_Scancode scancode) const noexcept {
    assert(catches(scancode));
    return keys_.at(scancode);
}

template <SDL_Scancode... scancodes>
void Key_catcher<scancodes...>::update(const SDL_Event& event) noexcept {
    if (event.type != SDL_KEYDOWN)
        return;

    clear();
    set(event.key.keysym.scancode);
}

template <SDL_Scancode... scancodes>
bool Key_catcher<scancodes...>::catches(SDL_Scancode scancode) const noexcept {
    return keys_.count(scancode) != 0;
}

template <SDL_Scancode... scancodes>
void Key_catcher<scancodes...>::clear() noexcept {
    if (!last_)
        return;

    keys_[*last_] = false;
}

template <SDL_Scancode... scancodes>
void Key_catcher<scancodes...>::set(SDL_Scancode scancode) noexcept {
    if (!catches(scancode))
        return;

    keys_[scancode] = true;
    last_ = scancode;
}

using Arrow_key_catcher = Technical::Key_catcher<SDL_SCANCODE_UP,
                                                 SDL_SCANCODE_DOWN,
                                                 SDL_SCANCODE_LEFT,
                                                 SDL_SCANCODE_RIGHT>;
}
