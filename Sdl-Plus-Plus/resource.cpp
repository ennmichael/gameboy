#include "resource.h"

namespace Sdl {
const char* Sdl_exception::what() const noexcept {
    return SDL_GetError();
}

Sdl_system_control::Sdl_system_control(Uint32 flags) {
    if (SDL_Init(flags) != 0)
        throw Sdl_exception{};
}

Sdl_system_control::~Sdl_system_control() {
    SDL_Quit();
}

void check_function(int function_result) {
    if (function_result != 0)
        throw Sdl_exception{};
}

void Window_deleter::operator()(SDL_Window* wnd_ptr) const noexcept {
    SDL_DestroyWindow(wnd_ptr);
}

void Renderer_deleter::operator()(SDL_Renderer* rdr_ptr) const noexcept {
    SDL_DestroyRenderer(rdr_ptr);
}

void Surface_deleter::operator()(SDL_Surface* surf_ptr) const noexcept {
    SDL_FreeSurface(surf_ptr);
}
}
