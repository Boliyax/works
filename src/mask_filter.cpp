#include "mask_filter.hpp"
#include <boost/regex.hpp>

MaskFilter::MaskFilter(const std::vector<std::string>& masks) {
    mask_vector.reserve(masks.size());
    for(const auto& i: masks) {
        mask_vector.push_back(boost::regex(i, boost::regex::perl | boost::regex::icase));
    }
}

std::vector<boost::filesystem::directory_entry> MaskFilter::run(std::vector<boost::filesystem::directory_entry>& input) {
    std::vector<boost::filesystem::directory_entry> rez;
    if (mask_vector.size() == 0) {
        for(const auto& i: input) {
            rez.push_back(i);
        }
    } else {
        for(const auto& i: input) {
            for(const auto& j: mask_vector) {
                if(boost::regex_match(i.path().filename().string(), j)) {
                    rez.push_back(i);
                    break;
                }
            }
        }
    }
    return rez;
}