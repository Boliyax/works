#ifndef PRINT_IP
#define PRINT_IP

#include <iostream>
#include <list>
#include <tuple>
#include <type_traits>
#include <vector>

/**
 * @defgroup Print_ip_group Вывод IP адресов
 * @brief Демонстрация работы механизма SFINAE на примере функции вывода IP
 *
 * @{
 */

#ifdef DOXYGEN_ONLY

/**
 * @brief Функция записи условного IP адреса в стандартный поток вывода
 * @details На вход функции поступает целочисленное число, строка, вектор,
 * список, или картеж. Функция выводит значение байтов разделенные точками, если
 * поступило число, необработанную строку, либо элементы контейнера разделенные
 * точками.
 *
 * @param ip условный IP адрес
 * @return void
 */
template <typename T> void print_ip(const T &ip);

#else

template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
void print_ip(const T &ip) {
  constexpr uint8_t BYTE = 8;
  auto &uip = static_cast<const std::make_unsigned_t<T> &>(ip);
  std::cout << (uip >> ((sizeof(T) - 1) * BYTE));
  for (size_t i = 1; i < sizeof(T); ++i) {
    std::cout << '.' << (uip << (i * BYTE) >> ((sizeof(T) - 1) * BYTE));
  }
  std::cout << std::endl;
}

template <typename T,
          std::enable_if_t<std::is_same_v<T, std::string>, bool> = true>
void print_ip(const T &ip) {
  std::cout << ip << std::endl;
}

template <
    typename T,
    std::enable_if_t<std::is_same_v<std::vector<typename T::value_type>, T> ||
                         std::is_same_v<std::list<typename T::value_type>, T>,
                     bool> = true>
void print_ip(const T &ip) {
  std::cout << *(ip.cbegin());
  for (auto i = std::next(ip.cbegin()); i != ip.cend(); ++i) {
    std::cout << '.' << *i;
  }
  std::cout << std::endl;
}

template <typename T, size_t N = 0,
          std::enable_if_t<std::is_same_v<typename std::tuple_element_t<N, T>,
                                          typename std::tuple_element_t<0, T>>,
                           bool> = true>
void print_ip(const T &ip) {
  if constexpr (N != 0) {
    std::cout << '.';
  }
  std::cout << std::get<N>(ip);
  if constexpr (N != std::tuple_size<T>() - 1) {
    print_ip<T, N + 1>(ip);
  } else {
    std::cout << std::endl;
  }
}

#endif // DOXYGEN_ONLY

/// @}

#endif // PRINT_IP
