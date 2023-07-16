#include "ip.hpp"
#include "split.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

std::vector<Ip> read_data() {
  std::vector<Ip> ret;
  while (true) {
    std::string input;
    std::getline(std::cin, input);
    if (input == "") {
      break;
    }
    ret.push_back(Ip(split(input, '\t')[0]));
  }
  return ret;
}
template <typename T, typename F> void filetr_print(const T &list, const F &P) {
  for (const auto &i : list) {
    if (P(i))
      std::cout << i << std::endl;
  }
}
int main() {
  auto ip_list = read_data();
  std::sort(ip_list.begin(), ip_list.end(),
            [](const Ip &a, const Ip &b) { return (a.addres > b.addres); });

  for (auto &i : ip_list) {
    std::cout << i << std::endl;
  }
  filetr_print(ip_list, [](const Ip &ip) { return ip.addres[0] == 1; });
  filetr_print(ip_list, [](const Ip &ip) {
    return (ip.addres[0] == 46 && ip.addres[1] == 70);
  });
  filetr_print(ip_list, [](const Ip &ip) {
    return (ip.addres[0] == 46 || ip.addres[1] == 46 || ip.addres[2] == 46 ||
            ip.addres[3] == 46);
  });

  return 0;
}
