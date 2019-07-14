#include "food.hpp"

#include <algorithm>
#include <iostream>

namespace Game_logic {
Sdl::Point_vector all_points() {
    Sdl::Point_vector result{};

    for (auto x = 0; x < Constants::game_size; ++x) {
        for (auto y = 0; y < Constants::game_size; ++y) {
            result.push_back({x, y});
        }
    }

    return result;
}

void remove_snake_points(Sdl::Point_vector& all, const Snake& snake) noexcept {
    all.erase(std::remove_if(all.begin(), all.end(),
                             [&snake](auto& elem) {
                                 return std::find(snake.points().cbegin(),
                                                  snake.points().cend(),
                                                  elem) !=
                                        snake.points().cend();
                             }),
              all.end());
}

void Food::reposition(const Snake& snake) noexcept {
    auto all = all_points();
    remove_snake_points(all, snake);

    std::uniform_int_distribution<std::size_t> dist{0, all.size() - 1};
    position_ = all[dist(dev_)];
}

Sdl::Point Food::position() const noexcept {
    return position_;
}
}
