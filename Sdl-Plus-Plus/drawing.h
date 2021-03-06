#pragma once

#include "SDL2/SDL.h"
#include "resource.h"
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <functional>
#include <cmath>

namespace Sdl {
struct Point;
struct Canvas;
struct Line;

using Point_vector = std::vector<Point>;

struct Point {
    int x;
    int y;
};

struct Size {  // TODO actually use this type 
    int width; // in the library
    int height;
};

constexpr bool operator==(Point, Point) noexcept;
constexpr bool operator!=(Point, Point) noexcept;
constexpr Point& operator+=(Point&, Point) noexcept;
constexpr Point& operator-=(Point&, Point) noexcept;
constexpr Point& operator*=(Point&, Point) noexcept;
constexpr Point& operator/=(Point&, Point) noexcept;
constexpr Point operator+(Point, Point) noexcept;
constexpr Point operator-(Point, Point) noexcept;
constexpr Point operator*(Point, Point) noexcept;
constexpr Point operator/(Point, Point) noexcept;
double distance(Point, Point) noexcept;

constexpr bool operator==(Size, Size) noexcept;
constexpr bool operator!=(Size, Size) noexcept;
constexpr Size& operator+=(Size&, Size) noexcept;
constexpr Size& operator-=(Size&, Size) noexcept;
constexpr Size& operator*=(Size&, Size) noexcept;
constexpr Size& operator/=(Size&, Size) noexcept;
constexpr Size operator+(Size, Size) noexcept;
constexpr Size operator-(Size, Size) noexcept;
constexpr Size operator*(Size, Size) noexcept;
constexpr Size operator/(Size, Size) noexcept;

enum class Message_box_type : Uint32 {
    Basic = 0,
    Error = SDL_MESSAGEBOX_ERROR,
    Warning = SDL_MESSAGEBOX_WARNING,
    Info = SDL_MESSAGEBOX_INFORMATION
};

struct Message_content {
    std::string title{};
    std::string text{};
};

void show_message(const Message_content&, Message_box_type);

constexpr SDL_Color color_red(Uint8 = 255, Uint8 alpha = 255) noexcept;
constexpr SDL_Color color_green(Uint8 = 255, Uint8 alpha = 255) noexcept;
constexpr SDL_Color color_blue(Uint8 = 255, Uint8 alpha = 255) noexcept;
constexpr SDL_Color color_yellow(Uint8 = 255, Uint8 alpha = 255) noexcept;
constexpr SDL_Color color_black(Uint8 alpha = 255) noexcept;
constexpr SDL_Color color_white(Uint8 alpha = 255) noexcept;

SDL_Rect make_rect(Sdl::Point, Sdl::Point) noexcept;
SDL_Rect make_rect(Sdl::Point, int w, int h) noexcept;

struct Screen_properties {  // Properties of a window
    int width{800};
    int height{600};
    Point position{SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED};
    std::string title{};
};

struct Canvas {  // A canvas is what you need in order to draw something
    Canvas() = default;
    explicit Canvas(const Screen_properties&);
    // Creates a canvas based on the given window properties

    Unique_window window{nullptr};
    Unique_renderer renderer{nullptr};
};

struct Angle_rad {  // Angle in radians
    double value{};
};

struct Line {
    Line() = default;
    Line(Point, Angle_rad, int) noexcept;
    Line(Point, Point) noexcept;

    Point from{};
    Point to{};
};

enum class Color_filling : bool { Filled, None };

class Screen {  // Manipulates a canvas
   public:
    explicit Screen(const Screen_properties&);

    void add_draw(Line, SDL_Color);
    void add_draw(SDL_Rect, SDL_Color, Color_filling);

    void redraw(SDL_Color);  // Clear, draw, render present

   private:
    using Snapshot = std::function<void(void)>;

    void set_renderer_color(SDL_Color);

    Canvas canvas_{};
    std::vector<Snapshot> snapshots_{};
};

// Basic_point operator definitions

constexpr bool operator==(Point lhs, Point rhs) noexcept {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

constexpr bool operator!=(Point lhs, Point rhs) noexcept {
    return !(lhs == rhs);
}

constexpr Point& operator+=(Point& lhs, Point rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

constexpr Point& operator-=(Point& lhs, Point rhs) noexcept {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

constexpr Point& operator*=(Point& lhs, Point rhs) noexcept {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

constexpr Point& operator/=(Point& lhs, Point rhs) noexcept {
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

constexpr Point operator+(Point lhs, Point rhs) noexcept {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

constexpr Point operator-(Point lhs, Point rhs) noexcept {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

constexpr Point operator*(Point lhs, Point rhs) noexcept {
    return {lhs.x * rhs.x, lhs.y * rhs.y};
}

constexpr Point operator/(Point lhs, Point rhs) noexcept {
    return {lhs.x / rhs.x, lhs.y / rhs.y};
}

constexpr bool operator==(Size lhs, Size rhs) noexcept {
    return lhs.width == rhs.width && 
           lhs.height == rhs.height;
}

constexpr bool operator!=(Size lhs, Size rhs) noexcept {
    return !(lhs == rhs);
}

constexpr Size& operator+=(Size& lhs, Size rhs) noexcept{
    lhs.width += rhs.width;
    lhs.height += rhs.height;
    return lhs;
}

constexpr Size& operator-=(Size& lhs, Size rhs) noexcept {
    lhs.width -= rhs.width;
    lhs.height -= rhs.height;
    return lhs;
}

constexpr Size& operator*=(Size& lhs, Size rhs) noexcept {
    lhs.width *= rhs.width;
    lhs.height *= rhs.height;
    return lhs;
}

constexpr Size& operator/=(Size& lhs, Size rhs) noexcept {
    lhs.width /= rhs.width;
    lhs.height /= rhs.height;
    return lhs;
}

constexpr Size operator+(Size lhs, Size rhs) noexcept {
    return {lhs.width + rhs.width, lhs.height + rhs.height};
}

constexpr Size operator-(Size lhs, Size rhs) noexcept {
    return {lhs.width - rhs.width, lhs.height - rhs.height};
}

constexpr Size operator*(Size lhs, Size rhs) noexcept {
    return {lhs.width * rhs.width, lhs.height * rhs.height};
}

constexpr Size operator/(Size lhs, Size rhs) noexcept {
    return {lhs.width / rhs.width, lhs.height / rhs.height};
}

constexpr SDL_Color color_red(Uint8 amount, Uint8 alpha) noexcept {
    return {amount, 0, 0, alpha};
}

constexpr SDL_Color color_green(Uint8 amount, Uint8 alpha) noexcept {
    return {0, amount, 0, alpha};
}

constexpr SDL_Color color_blue(Uint8 amount, Uint8 alpha) noexcept {
    return {0, 0, amount, alpha};
}

constexpr SDL_Color color_yellow(Uint8 amount, Uint8 alpha) noexcept {
    return {amount, amount, 0, alpha};
}

constexpr SDL_Color color_black(Uint8 alpha) noexcept {
    return {0, 0, 0, alpha};
}

constexpr SDL_Color color_white(Uint8 alpha) noexcept {
    return {255, 255, 255, alpha};
}
}

namespace std {
    template <>
    class hash<Sdl::Point> {
    public:
        std::size_t operator()(Sdl::Point p) const noexcept {
            return int_hash_(p.x) ^ int_hash_(p.y);
        }
        
    private:
        std::hash<int> int_hash_{};
    };
}
