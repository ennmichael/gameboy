#include "snake.hpp"
#include "constants.hpp"
#include <algorithm>
#include <utility>

namespace Game_logic {
void move(Snake_body& body) {
    body.points.pop_front();
    auto new_point = body.points.back() + direction_vector(body.dir);
    body.points.push_back(new_point);
}

Snake::Snake(Controller_uptr&& controller) noexcept
    : controller_{std::move(controller)} {}

Snake::Snake(const Snake& rhs, Controller_uptr&& controller) noexcept
    : controller_{std::move(controller)},
      body_{rhs.body_} {}

const Snake_points& Snake::points() const noexcept {
    return body_.points;
}

void Snake::grow() {
    body_.points.push_front(body_.points.front());
}

void Snake::frame_advance() noexcept {
    controller_->control(*this);
    move(body_);
    wrap_position();
}

void Snake::set_direction(Direction new_dir) noexcept {
    if (!opposite_direction(new_dir)) {
        body_.dir = new_dir;
    }
}

Sdl::Point head_pos(const Snake& snake) noexcept {
    return snake.points().back();
}

bool Snake::opposite_direction(Direction other_dir) const noexcept {
    return direction_vector(other_dir) ==
           direction_vector(body_.dir) * Sdl::Point{-1, -1};
}

void Snake::wrap_position() noexcept {
    wrap_coord(body_.points.back().x);
    wrap_coord(body_.points.back().y);
}

bool dead(const Snake& snake) noexcept {
    auto& points = snake.points();
    return std::count(points.cbegin(), points.cend(), head_pos(snake)) > 1;
}

std::size_t length(const Snake& snake) noexcept {
    return snake.points().size();
}

void wrap_coord(int& v) noexcept {
    if (v >= Constants::game_size)
        v -= Constants::game_size;
    else if (v < 0)
        v += Constants::game_size;
}
}

