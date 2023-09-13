#ifndef FILE_SEARCHER
#define FILE_SEARCHER

#include <vector>
#include <set>
#include <boost/filesystem.hpp>

class FileSearcher {
public:
    FileSearcher(std::vector<std::string>&, std::vector<std::string>&, unsigned int); 
    std::vector<boost::filesystem::directory_entry> run();
private:
    void find(std::vector<boost::filesystem::directory_entry>&, const boost::filesystem::directory_entry& target, unsigned int d);
    std::vector<std::string> scan;
    std::set<std::string> ignore;
    unsigned int depth;
    
};

#endif