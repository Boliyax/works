#include "ip.hpp"
#include "split.hpp"

#include <algorithm>
#include <numeric>

Ip::Ip(const std::string &s) {
  auto vec = split(s, '.');
  std::transform(vec.begin(), vec.end(), addres.begin(),
                 [](const std::string &s) { return std::stoi(s); });
}
Ip::operator const std::string() const {
  return std::accumulate(std::next(addres.begin()), addres.end(),
                         std::to_string(*(addres.begin())),
                         [](const std::string &s, const uint8_t &n) {
                           return s + '.' + std::to_string(n);
                         });
}

std::ostream &operator<<(std::ostream &stream, const Ip &ip) {
  return stream << static_cast<const std::string>(ip);
}