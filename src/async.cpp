#include "async.h"
#include <string>
#include <cstdlib>
#include <sstream>
#include "input_controller.hpp"
#include "output.hpp"


namespace async {

Output* output;
unsigned int counter = 0;
std::mutex connect_mutex;

// Теперь с разными контекстами можно работать из разных потоков.
// Но работа с отдельным контекстом должна происходить из одного потока.

handle_t connect(std::size_t bulk) {
    {
        std::lock_guard connect_guard(connect_mutex);
        if(counter == 0) {
            output = new Output();
        }
        ++counter;
    }
    auto p = output;
    return static_cast<void*>(new InputController(CommandSet(std::move(p), bulk)));
}

void receive(handle_t handle, const char *data, std::size_t size) {
    std::string s;
    for(size_t i = 0; i < size; ++i) {
        if((data[i] != '\n') && (data[i] != '\0')) {
            s+= data[i];
        } else if(s.size() > 0) {
            static_cast<InputController<CommandSet<Output*>>*>(handle)->message(s);
            s.clear();
        }
    }
    if(s.size() > 0) {
            static_cast<InputController<CommandSet<Output*>>*>(handle)->message(s);
    }
}

void disconnect(handle_t handle) {
    static_cast<InputController<CommandSet<std::unique_ptr<Output>>>*>(handle)->end();
    delete static_cast<InputController<CommandSet<Output*>>*>(handle);
    {
        std::lock_guard connect_guard(connect_mutex);
        --counter;
        if(counter == 0) {
            delete output;
        }
    }
}

}
