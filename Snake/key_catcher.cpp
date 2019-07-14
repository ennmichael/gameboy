#include "key_catcher.hpp"

namespace Game_logic {
bool Key_catcher::is_down(SDL_Scancode scancode) const noexcept {
    assert(catches(scancode));
    return keys_.at(scancode);
}

void Key_catcher::update(const SDL_Event& event) noexcept {
    if (event.type != SDL_KEYDOWN)
        return;

    clear();
    set(event.key.keysym.scancode);
}

bool Key_catcher::catches(SDL_Scancode scancode) const noexcept {
    return keys_.count(scancode) != 0;
}

void Key_catcher::clear() noexcept {
    if (!last_)
        return;

    keys_[*last_] = false;
}

void Key_catcher::set(SDL_Scancode scancode) noexcept {
    if (!catches(scancode))
        return;

    keys_[scancode] = true;
    last_ = scancode;
}
}
