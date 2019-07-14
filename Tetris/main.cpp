#include "game_logic.h"
#include "game.h"
#include "Sdl-Plus-Plus/drawing.h"
#include "Sdl-Plus-Plus/flow.h"
#include <chrono>
#include <iostream>

int main(int, char**) {
    using namespace Sdl;
    using namespace Game_logic;
    using namespace Visuals;
    using namespace std::literals::chrono_literals;

    Sdl_system_control _{};

    Screen_properties props{};
    props.width = window_width;
    props.height = window_height;
    props.title = "HELP";

    Screen screen{props};

    Game tetris{500ms};

    Event_loop ml{[&tetris](const SDL_Event& event) -> Event_result {
                      if (event.type == SDL_QUIT)
                          return Event_result::Quit;

                      tetris.frame_advance(event);
                      return Event_result::Continue;
                  },
                  [&screen, &tetris] {
                      tetris.frame_advance(screen);
                      screen.redraw(color_white());
                  }};

    ml.start();

    return 0;
}
