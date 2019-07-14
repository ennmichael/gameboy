#include "flow.h"
#include "SDL2/SDL.h"

namespace Sdl {
bool key_down(SDL_Scancode scancode) {
    return SDL_GetKeyboardState(nullptr)[scancode];
}

Event_loop::Event_loop(Event_function event_f,
                       Unconditional_function unconditional_f) noexcept
    : event_f_{event_f},
      unconditional_f_{unconditional_f} {}

void Event_loop::start() {
    while (true) {
        while (SDL_PollEvent(&event_)) {
            if (event_f_(event_) == Event_result::Quit)
                return;  // Quit if the user says so
        }

        unconditional_f_();
    }
}

Timer::Timer(std::chrono::milliseconds delay) noexcept {
    set_delay(delay);
}

bool Timer::ready() noexcept {
    auto ticks = SDL_GetTicks();

    if (ticks >= delay_ + current_) {
        current_ = ticks;
        return true;
    }

    return false;
}

std::chrono::milliseconds Timer::get_delay() const noexcept {
    return std::chrono::milliseconds{delay_};
}

void Timer::set_delay(std::chrono::milliseconds delay) noexcept {
    delay_ = static_cast<Uint32>(delay.count());
}
}
