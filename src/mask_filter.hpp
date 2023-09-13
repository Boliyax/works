#ifndef MASK_FILTER
#define MASK_FILTER

#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>


class MaskFilter {
public:
    MaskFilter(const std::vector<std::string>&);
    std::vector<boost::filesystem::directory_entry> run(std::vector<boost::filesystem::directory_entry>& input);
private:
    std::vector<boost::regex> mask_vector;
};

#endif