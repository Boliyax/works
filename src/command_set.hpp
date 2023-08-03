#include <string>
#include <list>
#include <fstream>
#include <fstream>
#include <ctime>

template<typename CALLBACK>
class CommandSet {
public:
    size_t size() const {return set.size();}
    void print();
    CommandSet(CALLBACK f, size_t n): callback(f), block_start(std::time(nullptr)), limit(n), set() {}
    void add_command(std::string& s) {
        set.emplace_back(s);
        if((level == 0) && (size() >= limit)) {
            callback(set, block_start);
            set.clear();
            block_start = std::time(nullptr);
        }
    }

    void end_input() {
        if((level == 0) && (size() >= 1)) {
            callback(set, block_start);
            set.clear();
            block_start = std::time(nullptr);
        }
    }

    void up() {
        if(level <= 0) {
            block_start = std::time(nullptr);
            if(size() > 0) {
                callback(set, block_start);
                set.clear();
            }
        }
        ++level;
    }
    void down() {
        if(--level <= 0) {
            callback(set, block_start);
            set.clear();
            block_start = std::time(nullptr);
        }
    }
    private:
    CALLBACK callback;
    time_t block_start = 0;
    size_t limit;
    size_t level = 0;
    std::list<std::string> set;
};