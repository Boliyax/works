#ifndef MAIN_MECHANISM
#define MAIN_MECHANISM

#include <vector>
#include <string>
#include "file_searcher.hpp"
#include "mask_filter.hpp"
#include "multi_comparator.hpp"

struct Options {
    std::vector<std::string> directories;
    std::vector<std::string> ignore;
    std::vector<std::string> masks;
    unsigned int block_size;
    unsigned int depth;
    unsigned int min_file_size;
    bool help = false;
};

class MainMechanism {
public:
    MainMechanism(Options&);
    std::vector<std::vector<std::string>> run();
private:
    FileSearcher searcher;
    MaskFilter filter;
    MultiComparator comparator;
};

#endif