#include <string>
#include <list>
#include <fstream>
#include <fstream>

template<typename CALLBACK>
class CommandSet {
public:
    size_t size() const {return set.size();}
    void print();
    CommandSet(CALLBACK&& f, size_t n): callback(std::move(f)), limit(n), set() {}
    void add_command(std::string& s) {
        set.emplace_back(s);
        if((level == 0) && (size() >= limit)) {
            //callback->print(set);
            mixed_print();
            set.clear();
        }
    }

    void end_input() {
        if((level == 0) && (size() >= 1)) {
            //callback->print(set);
            mixed_print();
            set.clear();
        }
    }

    void up() {
        if(level <= 0) {
            if(size() > 0) {
                //callback->print(set);
                mixed_print();
                set.clear();
            }
        }
        ++level;
    }
    void down() {
        if(--level <= 0) {
            callback->print(set);
            set.clear();
        }
    }
    void mixed_print() {
        callback->mix_print(set);
    }
    private:
    CALLBACK callback;
    size_t limit;
    size_t level = 0;
    std::list<std::string> set;
};