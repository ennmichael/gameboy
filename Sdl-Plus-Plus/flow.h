#pragma once

#include "drawing.h"
#include <functional>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <atomic>
#include <variant>

#include <iostream> // TEST

namespace Sdl {
bool key_down(SDL_Scancode);

enum class Event_result : bool { Quit, Continue };

class Event_loop {
   public:
    using Event_function = std::function<Event_result(SDL_Event)>;
    using Unconditional_function = std::function<void()>;

    Event_loop(Event_function, Unconditional_function) noexcept;
    void start();

   private:
    Event_function event_f_{};
    Unconditional_function unconditional_f_{};
    SDL_Event event_{};
};

class Timer {
   public:
    explicit Timer(std::chrono::milliseconds) noexcept;
    // The user is expected to give a sane value, larger than 0,
    // not overflowing Uint32. Because small values are pretty much expected,
    // none of this is actually checked, so beware or check it yourself.

    bool ready() noexcept;

    std::chrono::milliseconds get_delay() const noexcept;
    void set_delay(std::chrono::milliseconds) noexcept;

   private:
    Uint32 delay_{};
    Uint32 current_{SDL_GetTicks()};
};
}
