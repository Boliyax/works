#include "multi_comparator.hpp"

#include <map>
#include <fstream>
#include <forward_list>
#include <iterator>
#include <memory>
#include "hash.hpp"

MultiComparator::MultiComparator(unsigned int m, unsigned int b): alg(0), min(m), block(b) {}
unsigned int file_counter = 0;
std::vector<std::vector<boost::filesystem::directory_entry>> MultiComparator::run(std::vector<boost::filesystem::directory_entry>& input) {
    std::map<size_t, std::forward_list<boost::filesystem::directory_entry>> map;
    for(const auto& i: input) {
        auto size = boost::filesystem::file_size(i);
        if(size >= min) {
            if(map.contains(size)) {
                map[size].push_front(i);
            } else {
                std::forward_list<boost::filesystem::directory_entry> tmp;
                tmp.push_front(i);
                map.emplace(std::pair<size_t, std::forward_list<boost::filesystem::directory_entry>>({size, std::move(tmp)}));
            }
        }
    }
    std::unique_ptr<std::multimap<size_t, std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>>>> old (new std::multimap<size_t, std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>>>());
    for(auto& i: map) {
        if(std::next(i.second.begin()) == i.second.end()) {
                continue;
        }
        std::map<size_t, std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>>> tmp;
        for(auto& j: i.second) {
            char* buffer = new char[block];
            std::memset(buffer, 0, block);
            if(++file_counter > 510) {
                throw(std::overflow_error("too many similar files"));
            }
            auto stream = std::ifstream(j.path(), std::ios::binary);
            stream.read(buffer, block);
            size_t h = hash({buffer, block});
            if(tmp.contains(h)) {
                tmp[h].push_front({std::move(stream),std::move(j)});
            } else {
                std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>> ls;
                ls.push_front({std::move(stream),std::move(j)});
                tmp.emplace(std::pair<size_t, std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>>>(h, std::move(ls)));
            }
            
        }
        for(auto& j: tmp) {
            if(std::next(j.second.begin()) == j.second.end()) {
                j.second.begin()->first.close();
                --file_counter;
            }
        }
        old->merge(tmp);
    }
    std::vector<std::vector<boost::filesystem::directory_entry>> rez;
    while(old->size() > 0) {
        std::unique_ptr<std::multimap<size_t, std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>>>> now(new std::multimap<size_t, std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>>>());
        for(auto& i: *old) {
            if(std::next(i.second.begin()) == i.second.end()) {
                continue;
            }
            if(!i.second.front().first.good()) {
                std::vector<boost::filesystem::directory_entry> r;
                for(auto& j: i.second) {
                    r.push_back(std::move(j.second));
                }
                rez.push_back(std::move(r));
                continue;
            }
            std::map<size_t, std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>>> tmp;
            for(auto& j: i.second) {
                char* buffer = new char[block];
                std::memset(buffer, 0, block);
                j.first.read(buffer, block);
                size_t h = hash({buffer, block});
                if(tmp.contains(h)) {
                    tmp[h].push_front(std::move(j));
                } else {
                    std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>> ls;
                    ls.push_front(std::move(j));
                    tmp.emplace(std::pair<size_t, std::forward_list<std::pair<std::ifstream, boost::filesystem::directory_entry>>>(h, std::move(ls)));
                }
            }
            now->merge(tmp);
        }
        old = std::move(now);
    }
    return rez;    
}