#include "database.hpp"
#include <array>
#include <vector>
#include <string>
#include <map>

std::array<std::map<size_t, std::string>, 2> tables;

bool insert(int index, size_t id, std::string& value) {
    if(tables[index].count(id) == 0) {
        tables[index].insert(std::make_pair(id, value));
        return  true;
    }
    return false;
    
}
void truncate(int index) {
    tables[index].clear();

}

std::vector<std::tuple<size_t, std::string, std::string>> intersection() {
    std::vector<std::tuple<size_t, std::string, std::string>> rez;
    for(auto i: tables[0]) {
        if(tables[1].count(i.first) != 0) {
            rez.push_back(std::make_tuple(i.first, i.second, tables[1][i.first]));
        }
    }
    return rez;
}
std::vector<std::tuple<size_t, std::string, std::string>> symmetric_difference() {
    std::vector<std::tuple<size_t, std::string, std::string>> rez;
    auto a_i = tables[0].begin();
    auto b_i = tables[1].begin();
    auto a_e = tables[0].end();
    auto b_e = tables[1].end();
    while((a_i != a_e) && (b_i != b_e)) {
        if(a_i->first < b_i->first) {
            rez.push_back(std::make_tuple(a_i->first, a_i->second, std::string()));
            ++a_i;
            continue;
        }
        if(a_i->first > b_i->first) {
            rez.push_back(std::make_tuple(b_i->first, std::string(), b_i->second));
            ++b_i;
            continue;
        }
        ++a_i;
        ++b_i;

    }
    while(a_i != a_e) {
        rez.push_back(std::make_tuple(a_i->first, a_i->second, std::string()));
        ++a_i;
    }
    while(b_i != b_e) {
        rez.push_back(std::make_tuple(b_i->first, std::string(), b_i->second));
        ++b_i;
    }
    return rez;
}