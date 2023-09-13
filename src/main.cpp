#include <iostream>
#include <boost/program_options.hpp>
#include "main_mechanism.hpp"

int _CRT_glob = 0;

Options parse_args(int argc, char** argv) {
    Options options;
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help")
        ("scan,S",  boost::program_options::value<std::vector<std::string>>(&options.directories), "Directories from scanning")
        ("ignore,I",  boost::program_options::value<std::vector<std::string>>(&options.ignore), "Ignored directories")
        ("depth,D", boost::program_options::value<unsigned int>(&options.depth)->default_value(uint32_t(1)), "Depth of scanning")
        ("minimum,M", boost::program_options::value<unsigned int>(&options.min_file_size)->default_value(uint32_t(1)), "Minimal file size")
        ("masks,m",  boost::program_options::value<std::vector<std::string>>(&options.masks), "Masks of filename using perl syntax")
        ("block,B", boost::program_options::value<unsigned int>(&options.block_size)->default_value(uint32_t(8)), "Size of scanning file block");

    boost::program_options::variables_map map;
    boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    boost::program_options::store(parsed, map);
    boost::program_options::notify(map);
    if(map.count("help")) {
        options.help = true;
        std::cout << desc << std::endl;
    }
    return options;
}

template<typename T>
inline void print(T& arg) {
    for(auto& i: arg) {
        for(auto& j: i) {
            std::cout << j << std::endl;
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv) {
    auto options = parse_args(argc, argv);
    if(options.help) return 1;
    MainMechanism main_mechanism(options);
    auto result = main_mechanism.run();
    print(result);
    return 0;
}