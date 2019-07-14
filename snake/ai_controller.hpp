#ifndef _AI_CONTROLLER_HPP
#define _AI_CONTROLLER_HPP

#include "ai_controller.hpp"
#include "food.hpp"
#include "snake.hpp"
#include <optional>

namespace Game_logic {
enum class AI_state : bool { passive, aggressive };

struct Rule_data {
    const Snake& snake_;
    const Food& food_;
};

enum class Rule_priority : bool { mandatory, optional };

class Rule;

using Rule_uptr = std::unique_ptr<Rule>;

class Rule {
   public:
    Rule() = default;

    Rule(const Rule&) = delete;
    Rule(Rule&&) = delete;
    Rule& operator=(const Rule&) = delete;
    Rule& operator=(Rule&&) = delete;

    virtual ~Rule() = default;

    virtual bool okay(const Rule_data&) = 0;
    virtual Rule_priority priority() const = 0;
};

// Self_impact_rule:
// Don't allow self impact.
class Self_impact_rule : public Rule {
   public:
    bool okay(const Rule_data&) override;
    Rule_priority priority() const override;
};

// Food_distance_rule:
// Try to get closer to your food
class Food_distance_rule : public Rule {
   public:
    bool okay(const Rule_data&) override;
    Rule_priority priority() const override;

   private:
    // okay_impl:
    // expects to already have a value for previous_distance_,
    // and checks if the rule is okay.
    bool okay_impl(const Rule_data&) noexcept;

    std::optional<double> previous_distance_{std::nullopt};
};

// Breaking a lower level
// rule is acceptable, as long as it doesn't break a
// higher level rule.
// TODO what if we have two rules of the same priority?

using Rule_vector = std::vector<Rule_uptr>;

struct Rule_value {
    int mandatory_score_{};
    int optional_score_{};
};

// AI_controller:
// a controller for bot snakes.
// We'll probably have multiple variations of these.
class AI_controller : public Controller {
   public:
    AI_controller(const Food& rd, Rule_vector&&) noexcept;

    void think(Snake& self) override;
    //     Controller_uptr clone() const override;

   private:
    //     AI_controller(const AI_controller&) noexcept;

    Rule_value evaluate_case(Direction, const Snake&);
    void
    sort_rules() noexcept;  // Sorts the rules by priority in ascending order.
    void find_split() noexcept;  // Expects rules to be sorted.

    const Food& food_;
    Rule_vector rules_;
    Rule_vector::const_iterator split_;
    // split_:
    // this is where mandatory rules end and
    // optional rules start.
};
}

#endif
