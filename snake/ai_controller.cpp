#include "ai_controller.hpp"
#include "direction.hpp"
#include <array>
#include <cmath>
#include <functional>
#include <utility>

namespace Game_logic {
namespace {
auto distance(const Rule_data& rd) {
    auto square = [](double v) noexcept { return std::pow(v, 2); };

    auto diff = [](int v1, int v2) { return std::abs(v1 - v2); };

    auto food_pos = rd.food_.position();
    auto snake_pos = head_pos(rd.snake_);

    return std::sqrt(square(diff(food_pos.x, snake_pos.x)) +
                     square(diff(food_pos.y, snake_pos.y)));
}
}

bool Self_impact_rule::okay(const Rule_data& rd) {
    return !dead(rd.snake_);
}

Rule_priority Self_impact_rule::priority() const {
    return Rule_priority::mandatory;
}

bool Food_distance_rule::okay(const Rule_data& rd) {
    if (previous_distance_) {
        return okay_impl(rd);
    } else {
        previous_distance_ = distance(rd);
        return true;
    }
}

Rule_priority Food_distance_rule::priority() const {
    return Rule_priority::optional;
}

bool Food_distance_rule::okay_impl(const Rule_data& rd) noexcept {
    auto dist = distance(rd);
    auto result = dist < *previous_distance_;
    previous_distance_ = dist;

    return result;
}

AI_controller::AI_controller(const Food& food, Rule_vector&& rules) noexcept
    : food_{food},
      rules_{std::move(rules)} {
    sort_rules();
    find_split();
}

void AI_controller::think(Snake& self) {
    constexpr std::array<Direction, 4> directions{
        {Direction::up, Direction::down, Direction::left, Direction::right}};

    Rule_value best_case{};
    Direction result{};

    for (auto dir : directions) {
        auto new_case = evaluate_case(dir, self);

        if (new_case.mandatory_score_ > best_case.mandatory_score_ ||
            (new_case.mandatory_score_ == best_case.mandatory_score_ &&
             new_case.optional_score_ > best_case.optional_score_)) {
            best_case = new_case;
            result = dir;
        }
    }

    self.set_direction(result);

    Sdl::Message_content content;
    content.title = "";
    content.text = "Pause";
}

Rule_value AI_controller::evaluate_case(Direction dir, const Snake& self) {
    Rule_value rv{};

    Snake modified_snake{self, std::make_unique<Dummy_controller>()};
    modified_snake.set_direction(dir);
    modified_snake.frame_advance();

    auto head = head_pos(modified_snake);

    const Rule_data rd{modified_snake, food_};

    for (auto i = rules_.cbegin(); i != split_; ++i) {
        if ((*i)->okay(rd)) {
            ++rv.mandatory_score_;
        }
        // These are mandatory
    }

    for (auto i = split_; i != rules_.cend(); ++i) {
        if ((*i)->okay(rd)) {
            ++rv.optional_score_;
        }
    }

    return rv;
}

void AI_controller::sort_rules() noexcept {
    std::sort(rules_.begin(), rules_.end(),
              [](const auto& rule1, const auto& rule2) {
                  return rule1->priority() < rule2->priority();
              });
}

void AI_controller::find_split() noexcept {
    split_ = std::find_if(
        rules_.cbegin(), rules_.cend(),
        [](const auto& e) {  // Basically find the first optional rule
            return e->priority() == Rule_priority::optional;
        });
}
}
