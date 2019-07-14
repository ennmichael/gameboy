#include "game_logic.h"
#include "game.h"
#include "Sdl-Plus-Plus/drawing.h"
#include "Sdl-Plus-Plus/flow.h"
#include <chrono>
#include <iostream>

int main(int, char**) {
    using namespace std::chrono_literals;

    Sdl::Sdl_system_control _{};

    Sdl::Screen_properties props{};
    props.width = Visuals::window_width;
    props.height = Visuals::window_height;
    props.title = "HELP";

    Sdl::Screen screen{props};

    Game_logic::Game tetris{500ms};

    Sdl::Event_loop ml{[&tetris](const auto& event) -> Sdl::Event_result {
                           if (event.type == SDL_QUIT)
                               return Sdl::Event_result::Quit;

                           tetris.frame_advance(event);
                           return Sdl::Event_result::Continue;
                       },
                       [&screen, &tetris] {
                           tetris.frame_advance(screen);
                           screen.redraw(Sdl::color_white());
                       }};

    ml.start();

    return 0;
}
