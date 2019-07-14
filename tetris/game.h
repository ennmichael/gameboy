#ifndef _GAME_H__
#define _GAME_H__

#include "game_logic.h"
#include "visuals.h"
#include <map>
#include <functional>
#include <chrono>

/*
 * The game implementation,
 * basically the implementation of tha game's
 * main loop. This file combines the game logic
 * and visuals.
 */

namespace Game_logic {
enum class Drop_mode : bool { Normal, Fast };

class Game_timer {
   public:
    explicit Game_timer(std::chrono::milliseconds) noexcept;

    static constexpr std::chrono::milliseconds min() noexcept {
        using namespace std::chrono_literals;
        return 50ms;
    }

    static constexpr std::chrono::milliseconds speed_boost() noexcept {
        using namespace std::chrono_literals;
        return 10ms;
    }

    bool ready() noexcept;
    void set_mode(Drop_mode) noexcept;
    void speed_up() noexcept;

   private:
    std::chrono::milliseconds delay_;
    Sdl::Timer timer_;
};

struct Button_dispatcher_data;

/*
 * Button_dispatcher:
 * a function class used to handle button presses.
 */
class Button_dispatcher {
   public:
    Button_dispatcher();
    void operator()(const SDL_Event&, Button_dispatcher_data) noexcept;

   private:
    using Dispatch_function =
        std::function<void(Button_dispatcher_data) noexcept>;
    using Dispatch_map = std::map<SDL_Scancode, Dispatch_function>;

    void do_dispatch(Dispatch_map&,
                     Button_dispatcher_data,
                     const SDL_Scancode&) noexcept;

    Dispatch_map keydown_dispatch_;
    Dispatch_map keyup_dispatch_;
};

/*
 * Button_dispatcher_data:
 * holds the data that may be modified when the user
 * presses/releases a button, depending on which button it was.
 */
struct Button_dispatcher_data {
    Tetromino& tetromino_;
    const Tetromino_table& tbl_;
    /*
     * This field is const because pressing a button
     * should never modify the table directly
     */
    Game_timer& game_timer_;
};

/*
 * Game:
 * represents the whole game process,
 * where the game advances a frame with
 * each call to frame_advance().
 */
class Game {
   public:
    explicit Game(std::chrono::milliseconds) noexcept;

    void frame_advance(const SDL_Event&);
    void frame_advance(Sdl::Screen&);

   private:
    void check_timer() noexcept;
    void handle_event(const SDL_Event&);
    void insert_tetromino() noexcept;
    void reset() noexcept;
    void check_game_over() noexcept;
    void handle_rows() noexcept;
    Tetromino create_tetromino() noexcept;

    Game_timer timer_;
    Button_dispatcher dispatch_button_{};
    Tetromino_factory t_factory_{};
    Tetromino_table t_table_{};
    Tetromino current_tetromino_{create_tetromino()};
    int score_{0};
    bool game_over_{false};
};
}

#endif
