#pragma once

#include <memory>

namespace Game_logic {
class Controller;
class Snake;

using Controller_uptr = std::unique_ptr<Controller>;

/*
 * Controller:
 * used for implementing the behaviour of a snake.
 * Member function control is called each frame, right
 * before the snake advances a frame.
 */
class Controller {
   public:
    Controller() = default;

    Controller(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller& operator=(Controller&&) = delete;

    virtual ~Controller() = default;

    virtual void control(Snake& self) = 0;
};

class Dummy_controller : public Controller {
   public:
    void control(Snake&) override {}
};
}
