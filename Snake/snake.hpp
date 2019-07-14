#pragma once

#include "controller.hpp"
#include "direction.hpp"
#include <deque>

namespace Game_logic {
using Snake_points = std::deque<Sdl::Point>;

struct Snake_body {
    Snake_points points{};
    Direction dir{};
};

void move(Snake_body&);

class Snake {
   public:
    explicit Snake(Controller_uptr&&) noexcept;
    Snake(const Snake&, Controller_uptr&&) noexcept;

    const Snake_points& points() const noexcept;

    void grow();
    void frame_advance() noexcept;
    void set_direction(Direction) noexcept;

   private:
    bool opposite_direction(Direction) const noexcept;
    void wrap_position() noexcept;

    Controller_uptr controller_;
    Snake_body body_{{{0, 0}, {1, 0}}, Direction::right};
};

Sdl::Point head_pos(const Snake&) noexcept;
bool dead(const Snake&) noexcept;
std::size_t length(const Snake&) noexcept;
void wrap_coord(int&) noexcept;
}  // End namespace Game_logic
