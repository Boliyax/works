#include <string>
#include <vector>

bool insert(int index, size_t id, std::string& value);
void truncate(int index);
std::vector<std::tuple<size_t, std::string, std::string>> symmetric_difference();
std::vector<std::tuple<size_t, std::string, std::string>> intersection();
