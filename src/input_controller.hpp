#include <string>
#include "command_set.hpp"


template<typename BLC_CONTROLLER>
class InputController {
public:
    void message(std::string& ms) {
        if(ms == "{") {
            set.up();
            return;
        }
        if(ms == "}") {
            set.down();
            return;
        }
        set.add_command(ms);
    }
    void end() {
        set.end_input();
    }
    InputController(BLC_CONTROLLER&& cs): set(std::move(cs)) {}
private:
    BLC_CONTROLLER set;
};