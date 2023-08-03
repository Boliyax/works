#include <iostream>
#include <string>
#include <cstdlib>
#include "input_controller.hpp"
#include "output.hpp"

int main(int argc, char** argv) {
    size_t n;
    if(argc == 2) {
        n = atoi(argv[1]);
    } else {
        n = 1;
    }
    auto controller = InputController(CommandSet(&file_n_console_print, n));
    std::string s;
    while(std::getline(std::cin, s)) {
        controller.message(s);
    }
    controller.end();
}