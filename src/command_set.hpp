#include <string>
#include <list>
#include <fstream>
#include <fstream>

template<typename CALLBACK>
class CommandSet {
public:
    size_t size() const {return _size;}
    void print();
    CommandSet(CALLBACK&& f, size_t n): callback(std::move(f)), limit(n), set() {}
    void add_command(std::string& s) {
        if(level == 0) {
            ++_size;
            callback->add_static_command(s);
        } else {
            set.emplace_back(s);
        }
        if((level == 0) && (size() >= limit)) {
            //callback->print(set);
            mixed_print();
        }
    }

    void end_input() {
        mixed_print();
        set.clear();
    }

    void up() {
        if(level <= 0) {
            mixed_print();
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
        callback->mix_print(_size);
        _size = 0;
    }
    private:
    CALLBACK callback;
    size_t _size = 0;
    size_t limit;
    size_t level = 0;
    std::list<std::string> set;
};