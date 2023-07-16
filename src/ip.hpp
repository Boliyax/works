#ifndef MY_IP_CLASS
#define MY_IP_CLASS

#include <array>
#include <iostream>
#include <string>


class Ip {
public:
  std::array<uint8_t, 4> addres;

  Ip(const std::string &);
  operator const std::string() const;
};

std::ostream &operator<<(std::ostream &, const Ip &);

#endif