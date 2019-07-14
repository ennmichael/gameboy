#include "game.hpp"

namespace Game_logic {
bool eats(const Snake& snake, const Food& food) noexcept {
    return head_pos(snake) == food.position();
}

Game::Game(std::chrono::milliseconds ms) noexcept : timer_{ms} {
    food_.reposition(player_);
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

    player_.frame_advance();
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
    if (!eats(player_, food_))
        return;

    player_.grow();
    food_.reposition(player_);
}

void Game::check_dead() noexcept {
    if (!dead(player_))
        return;

    died_ = true;

    Sdl::Message_content content{};
    content.title = "Game over!";
    content.text = "Final score: " + std::to_string(length(player_));

    Sdl::show_message(content, Sdl::Message_box_type::Basic);
}

void Game::redraw(Sdl::Screen& screen) {
    Visuals::draw(screen, player_);
    Visuals::draw(screen, food_);
    screen.redraw(Constants::background_color);
}
}  // End namespace Game_logic.
