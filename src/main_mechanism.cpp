#include "main_mechanism.hpp"

MainMechanism::MainMechanism(Options& options): 
    searcher(options.directories, options.ignore, options.depth),
    filter(options.masks),
    comparator(options.min_file_size, options.block_size)
    {}

std::vector<std::vector<std::string>> MainMechanism::run() {
    auto files = searcher.run();
    auto filtered_files = filter.run(files);
    auto final_files = comparator.run(filtered_files);
    std::vector<std::vector<std::string>> rez{};
    for(const auto& i: final_files) {
        rez.emplace_back();
        rez.back().reserve(i.size());
        for(const auto& j: i) {
            rez.back().push_back(j.path().string());
        }
    }
    return rez;
}