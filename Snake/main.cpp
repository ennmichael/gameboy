#include "constants.hpp"
#include "game.hpp"
#include "Sdl-Plus-Plus/drawing.h"
#include "Sdl-Plus-Plus/flow.h"
#include "snake.hpp"
#include "visuals.hpp"

int main(int argc, char** argv) {
    using namespace Sdl;
    using namespace Visuals;
    using namespace Game_logic;
    using namespace Constants;

    using namespace std::chrono_literals;
    using namespace std::string_literals;

    auto screen = get_screen();

    auto const controlled_by = (argc > 1 && std::string{argv[1]} == "--ai"s) ? Game::Controlled_by::AI : Game::Controlled_by::Player;
    Game game{controlled_by, 100ms};

    Event_loop loop{[&screen, &game](const auto& event) -> Sdl::Event_result {
                        if (event.type == SDL_QUIT)
                            return Event_result::Quit;

                        game.handle_event(event);

                        return Event_result::Continue;
                    },
                    [&screen, &game] { game.frame_advance(screen); }};

    loop.start();

    return 0;
}
