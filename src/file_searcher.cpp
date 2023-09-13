#include "file_searcher.hpp"

FileSearcher::FileSearcher(std::vector<std::string>& sc, std::vector<std::string>& ig, unsigned int d): scan(sc), ignore(), depth(d) {
    for(const auto& i: ig) {
        ignore.emplace(i);
    }
}

void FileSearcher::find(std::vector<boost::filesystem::directory_entry>& vec, const boost::filesystem::directory_entry& target, unsigned int d) {
    boost::filesystem::directory_iterator end;
    for (boost::filesystem::directory_iterator begin(target);  begin != end; ++begin) {
            if((boost::filesystem::is_directory(*begin)) && (d > 1) && (!ignore.contains(begin->path().filename().string()))) {
                find(vec, *begin, d - 1);
            }
            else if(boost::filesystem::is_regular_file(*begin)) {
                vec.push_back(*begin);
            }
        }
}

std::vector<boost::filesystem::directory_entry> FileSearcher::run() {
    std::vector<boost::filesystem::directory_entry> rez;
    for (const auto& i: scan) {
        find(rez, boost::filesystem::directory_entry(i), depth);
    }
    return rez;

}