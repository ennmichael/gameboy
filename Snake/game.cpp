#include "game.hpp"

namespace Game_logic {
bool eats(const Snake& snake, const Food& food) noexcept {
    return head_pos(snake) == food.position();
}

Game::Game(Controlled_by controlled_by, std::chrono::milliseconds ms) : timer_{ms} {
    if (controlled_by == Controlled_by::Player) {
        snake_ = Snake{std::make_unique<Player_controller>(keys_)};
    } else if (controlled_by == Controlled_by::AI) {
        Rule_vector rules{};
        rules.push_back(std::make_unique<Self_impact_rule>());
        rules.push_back(std::make_unique<Food_distance_rule>());
        snake_ = Snake{std::make_unique<AI_controller>(food_, std::move(rules))};
    }
}

void Game::frame_advance(Sdl::Screen& screen) {
    if (died_) {
        // The game freezes after game-over, so we
        // need to make sure the screen never glitches out,
        // hence the call to redraw here.
        redraw(screen);
        return;
    }

    if (!timer_.ready())
        return;

    snake_.frame_advance();
    check_interaction();
    redraw(screen);

    check_dead();
}

void Game::handle_event(const SDL_Event& event) noexcept {
    if (died_)
        return;

    keys_.update(event);
}

void Game::check_interaction() {
    if (!eats(snake_, food_))
        return;

    snake_.grow();
    food_.reposition(snake_);
}

void Game::check_dead() noexcept {
    if (!dead(snake_))
        return;

    died_ = true;

    Sdl::Message_content content{};
    content.title = "Game over!";
    content.text = "Final score: " + std::to_string(length(snake_));

    Sdl::show_message(content, Sdl::Message_box_type::Basic);
}

void Game::redraw(Sdl::Screen& screen) {
    Visuals::draw(screen, snake_);
    Visuals::draw(screen, food_);
    screen.redraw(Constants::background_color);
}
}  // End namespace Game_logic.
