#ifndef MULTI_COMPARATOR
#define MULTI_COMPARATOR

#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <exception>

class MultiComparator {
public:
    MultiComparator(unsigned int, unsigned int);
    std::vector<std::vector<boost::filesystem::directory_entry>> run(std::vector<boost::filesystem::directory_entry>& );

    unsigned int alg;
    unsigned int min;
    unsigned int block;

};

#endif