#include "command_interpreter.hpp"
#include "errors.hpp"
#include "database.hpp"
#include <vector>
#include <cstdlib>
#include <execution>

std::vector<std::string> split(std::string& s) {
    std::vector<std::string> rez;
    size_t i = 0;
    while(i < s.size()) {
        size_t e = s.find(' ', i);
        if(e == std::string::npos) e = s.size();
        rez.push_back(s.substr(i, e - i));
        i = e + 1;
    }
    return rez;
}

int index_from_table_name(std::string& s) {
    if(s == "A") return 0;
    if(s == "B") return 1;
    return -1;
}



std::string interpret(std::string& s) {
    auto v = split(s);
    if(v[0] == "INSERT") {
        if(v.size() != 4) return er::er_s + " " + er::comm_size;
        auto index = index_from_table_name(v[1]);
        if(index == -1) return er::er_s + " " + er::ukn_tab;
        long int id;
        try {
            id = std::stoi(v[2]);
        } catch( std::exception&) {
            return er::er_s + " " + er::inv_arg;
        }
        if(id < 0) {
            return er::er_s + " " + er::inv_arg;
        }
        std::string value = v[3];
        if(!insert(index, id, value))  {
            return er::er_s + " " + er::not_un + " " + std::to_string(id);
        }
        return "OK";

    } else if (v[0] == "TRUNCATE") {
        if(v.size() != 2) return er::er_s + " " + er::comm_size;
        auto index = index_from_table_name(v[1]);
        if(index == -1) return er::er_s + " " + er::ukn_tab;
        truncate(index);
        return "OK";

    } else if (v[0] == "INTERSECTION") {
        if(v.size() != 1) return er::er_s + " " + er::comm_size;
        auto inter = intersection();
        std::string rez;
        for(auto& i: inter) {
            rez += std::to_string(std::get<0>(i)) + "," + std::get<1>(i) + "," + std::get<2>(i) + "\n";
        }
        return rez + "OK";


    } else if (v[0] == "SYMMETRIC_DIFFERENCE") {
        if(v.size() != 1) return er::er_s + " " + er::comm_size;
        auto inter = symmetric_difference();
        std::string rez;
        for(auto& i: inter) {
            rez += std::to_string(std::get<0>(i)) + "," + std::get<1>(i) + "," + std::get<2>(i) + "\n";
        }
        return rez + "OK";

    } else {
        return er::er_s + " " + er::command;
    }


}