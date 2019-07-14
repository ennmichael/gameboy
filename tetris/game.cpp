#include "game.h"

namespace Game_logic {
Game_timer::Game_timer(std::chrono::milliseconds delay) noexcept
    : delay_{delay},
      timer_{delay} {}

bool Game_timer::ready() noexcept {
    return timer_.ready();
}

void Game_timer::set_mode(Drop_mode mode) noexcept {
    if (mode == Drop_mode::Normal) {
        timer_.set_delay(delay_);
    } else if (mode == Drop_mode::Fast) {
        timer_.set_delay(min());
    }
}

void Game_timer::speed_up() noexcept {
    if (delay_ > min()) {
        delay_ -= speed_boost();
    }
}

Button_dispatcher::Button_dispatcher()
    : keydown_dispatch_{
        {
            SDL_SCANCODE_LEFT,
            [](auto dispatcher_data) noexcept -> void {
                safe_move(dispatcher_data.tetromino_,
                dispatcher_data.tbl_,
                {-1, 0});
            }
        },
        {
            SDL_SCANCODE_RIGHT,
            [](auto dispatcher_data) noexcept {
                safe_move(dispatcher_data.tetromino_, dispatcher_data.tbl_, {1, 0});
            }
        },
        {
            SDL_SCANCODE_X,
            [](auto dispatcher_data) noexcept {
                safe_rotate(dispatcher_data.tetromino_, dispatcher_data.tbl_, Rotation::Clockwise);
            }
        },
        {
            SDL_SCANCODE_Y,
            [](auto dispatcher_data) noexcept {
                safe_rotate(dispatcher_data.tetromino_, dispatcher_data.tbl_, Rotation::Counter_clockwise);
            }
        },
        {
            SDL_SCANCODE_DOWN,
            [](auto dispatcher_data) noexcept {
                dispatcher_data.game_timer_.set_mode(Drop_mode::Fast);
            }
        }
    }
    , keyup_dispatch_{
        {
            SDL_SCANCODE_DOWN,
            [](auto dispatcher_data) noexcept {
                dispatcher_data.game_timer_.set_mode(Drop_mode::Normal);
            }
        }
    }
{
    // Making sure that the same button does the same thing
    // on both English and German keyboards.
    keydown_dispatch_[SDL_SCANCODE_Z] = keydown_dispatch_[SDL_SCANCODE_Y];
}

void Button_dispatcher::operator()(
    const SDL_Event& event,
    Button_dispatcher_data dispatcher_data) noexcept {
    if (event.type == SDL_KEYDOWN) {
        do_dispatch(keydown_dispatch_, dispatcher_data,
                    event.key.keysym.scancode);
    } else if (event.type == SDL_KEYUP) {
        do_dispatch(keyup_dispatch_, dispatcher_data,
                    event.key.keysym.scancode);
    }
}

void Button_dispatcher::do_dispatch(Dispatch_map& dm,
                                    Button_dispatcher_data dispatcher_data,
                                    const SDL_Scancode& scancode) noexcept {
    if (dm.count(scancode) == 0)
        return;

    dm[scancode](dispatcher_data);
}

Game::Game(std::chrono::milliseconds delay) noexcept : timer_{delay} {}

void Game::frame_advance(const SDL_Event& event) {
    handle_event(event);
}

void Game::frame_advance(Sdl::Screen& screen) {
    check_timer();
    Visuals::draw_game_element(screen, current_tetromino_);
    Visuals::draw_game_element(screen, t_table_);
}

void Game::check_timer() noexcept {
    if (game_over_)
        return;

    if (!timer_.ready())
        return;

    if (touches_ground(current_tetromino_) ||
        touches_table(t_table_, current_tetromino_, {0, 1})) {
        insert_tetromino();
        handle_rows();
        reset();
        check_game_over();
        return;
    }

    current_tetromino_.pos_ += Sdl::Point{0, 1};
}

void Game::handle_event(const SDL_Event& event) {
    dispatch_button_(event, {current_tetromino_, t_table_, timer_});
}

void Game::insert_tetromino() noexcept {
    t_table_.insert(current_tetromino_);
}

void Game::reset() noexcept {
    current_tetromino_ = create_tetromino();
}

void Game::check_game_over() noexcept {
    if (table_overlaps_tetromino(t_table_, current_tetromino_)) {
        game_over_ = true;

        Sdl::Message_content content{};
        content.title = "Game over!";
        content.text = "Final score: " + std::to_string(score_) + ".\n";

        Sdl::show_message(content, Sdl::Message_box_type::Basic);
    }
}

void Game::handle_rows() noexcept {
    auto cleared_rows = t_table_.clear_rows();
    score_ += cleared_rows;

    for (int _ = 0; _ < cleared_rows; ++_) {
        timer_.speed_up();
    }
}

Tetromino Game::create_tetromino() noexcept {
    return t_factory_.create({table_width / 2, 0});
}
}
