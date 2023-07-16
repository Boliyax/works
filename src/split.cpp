#include "split.hpp"

std::vector<std::string> split(const std::string &s, const char c) {
  std::vector<std::string> ret{};
  size_t iter = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == c) {
      ret.push_back(s.substr(iter, i - iter));
      iter = i + 1;
    }
  }
  ret.push_back(s.substr(iter, s.size() - iter));
  return ret;
}