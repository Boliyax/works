#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>


void print_list(std::list<std::string>& set, std::ostream& stream) {

    stream << "bulk: " << *set.begin();
    for(auto i = std::next(set.begin()); i != set.end(); ++i) {
        stream << ", " << *i;
    }
    stream << std::endl;
}

void file_n_console_print(std::list<std::string>& set, time_t time) {
    std::ofstream f("bulk" + std::to_string(time) + ".log");
    print_list(set, static_cast<std::ostream&>(f));
    print_list(set, static_cast<std::ostream&>(std::cout));

}
