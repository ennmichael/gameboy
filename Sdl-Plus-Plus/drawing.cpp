#include "drawing.h"
#include <cmath>
#include <iostream>

namespace Sdl {
double distance(Point p1, Point p2) noexcept {
    auto diff = [](auto v1, auto v2) {
        return std::abs(v1 - v2);
    };
    
    return std::sqrt(
        std::pow(diff(p1.x, p2.x), 2) + std::pow(diff(p1.y, p2.y), 2)
    );
}

// TODO fix passing unique pointer references around,
// start passing actual references to the required resources
void show_message(const Message_content& content, Message_box_type type) {
    check_function(SDL_ShowSimpleMessageBox(static_cast<Uint32>(type),
                                            content.title.c_str(),
                                            content.text.c_str(), nullptr));
}

SDL_Rect make_rect(Sdl::Point upper_left, Sdl::Point lower_right) noexcept {
    return {upper_left.x, upper_left.y, lower_right.x - upper_left.x,
            lower_right.y - upper_left.y};
}

SDL_Rect make_rect(Sdl::Point upper_left, int w, int h) noexcept {
    return {upper_left.x, upper_left.y, w, h};
}

Canvas::Canvas(const Screen_properties& properties)
    : window{SDL_CreateWindow(properties.title.c_str(),
                              properties.position.x,
                              properties.position.y,
                              properties.width,
                              properties.height,
                              SDL_WINDOW_SHOWN)},
      renderer{SDL_CreateRenderer(window.get(), -1, 0)} {
    check_pointer(window);
    check_pointer(renderer);
}

Line::Line(Point new_from, Angle_rad rads, int length) noexcept
    : from{new_from},
      to{static_cast<int>(std::sin(rads.value) * length + from.x),
         static_cast<int>(std::cos(rads.value) * length + from.y)}
// Sdl::Point_vector are integers in the SDL library, so narrowing
{  // has to happen
}

Line::Line(Point new_from, Point new_to) noexcept : from{new_from},
                                                    to{new_to} {}

Screen::Screen(const Screen_properties& properties) : canvas_{properties} {
    check_function(SDL_SetRenderDrawBlendMode(canvas_.renderer.get(),
                                              SDL_BLENDMODE_BLEND));
}

void Screen::add_draw(Line line, SDL_Color color) {
    snapshots_.push_back([this, line, color] {
        set_renderer_color(color);

        check_function(SDL_RenderDrawLine(canvas_.renderer.get(), line.from.x,
                                          line.from.y, line.to.x, line.to.y));
    });
}

void Screen::add_draw(SDL_Rect rect, SDL_Color color, Color_filling filling) {
    snapshots_.push_back([this, rect, color, filling] {
        set_renderer_color(color);

        check_function(SDL_RenderDrawRect(canvas_.renderer.get(), &rect));
        
        if (filling == Color_filling::Filled) {
            check_function(SDL_RenderFillRect(canvas_.renderer.get(), &rect));
        }
    });
}

void Screen::redraw(SDL_Color color) {
    set_renderer_color(color);
    check_function(SDL_RenderClear(canvas_.renderer.get()));

    for (const auto& snapshot : snapshots_)
        snapshot();

    SDL_RenderPresent(canvas_.renderer.get());
    snapshots_.clear();
}

void Screen::set_renderer_color(SDL_Color color) {
    check_function(SDL_SetRenderDrawColor(canvas_.renderer.get(), color.r,
                                          color.g, color.b, color.a));
}
}
