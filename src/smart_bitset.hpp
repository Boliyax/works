#ifndef SMART_BITSET
#define SMART_BITSET

#include <bitset>

// Менеджер пула использующий битовую арифметику, хранит информацию о
// занятых ячейках в массиве битов. 1-ей кодируется свободная память
template <size_t SET_SIZE> class SmartBitSet {

  using bitset = std::bitset<SET_SIZE>;

  bitset data;
  size_t unresolved_length; // Хранит длину непрерывного ряда незанятых ячеек
                            // которого нет в пуле. Сбрасывается при
                            // освобождении памяти. Используется для быстрого
                            // отсечения неудачных запросов.

  // Метод для "склеивания" соседних сегментов. Полученный биты обозначают
  // непрерывные ряды свободных ячеек нужной длины.
  static inline void and_up(bitset &bits, const size_t &offset) noexcept {
    bits &= (bits >> offset);
  }

  // Метод для "поднятия" сегментов. Полученный биты обозначают
  // что под ними находятся нужные нам ряды свободных ячеек.
  static inline bitset new_or_up(const bitset &bits,
                                 const size_t &offset) noexcept {
    return bits | (bits >> offset);
  }

  // Резервирование или освобождения ячеек, в зависимости от контекста.
  inline void flip_bits(const size_t &position, const size_t &length) noexcept {
    data ^= bitset().set() >> (SET_SIZE - length) << position;
  }

  // Шаблонные методы выстраивающие подобие бинарного дерева, для поиска
  // свободного ряда с наименьшим индексом.
  template <size_t OFFSET = 1>
  static inline size_t finde_first(const bitset &bits) noexcept {
    if constexpr (OFFSET << 1 < SET_SIZE) {
      auto position = finde_first<OFFSET << 1>(new_or_up(bits, OFFSET));
      if (bits.test(position)) {
        return position;
      } else {
        return position + OFFSET;
      }
    }
    if constexpr (OFFSET << 1 >= SET_SIZE) {
      if (bits.test(0)) {
        return 0;
      } else {
        return OFFSET;
      }
    }
    return 0;
  }

public:
  SmartBitSet() noexcept : data(), unresolved_length(SET_SIZE + 1) {
    data.set();
  }

  size_t acquire(size_t length) noexcept {

    if (length >= unresolved_length) {
      return SET_SIZE; // Нужного размера ряд не найден.
    }

    auto bits = data;
    if (length == 1) {
      auto position = finde_first(bits);
      if (!data.test(position))
        return SET_SIZE;
      flip_bits(position, length);
      return position;
    }
    for (size_t i = 1;; i <<= 1) { // Цикл "склеивания" битов
      if (length <= i << 1) {
        auto final_offset = length - (i);
        and_up(bits, final_offset);
        if (!bits.any()) {
          unresolved_length = length;
          return SET_SIZE;
        } else {
          auto position = finde_first(bits);
          flip_bits(position, length);
          return position;
        }
      } else {
        and_up(bits, i);
        if (!bits.any()) {
          unresolved_length = i << 2;
          return SET_SIZE;
        }
      }
    }
    return SET_SIZE;
  }

  void release(size_t position, size_t length) noexcept {
    unresolved_length = SET_SIZE + 1;
    flip_bits(position, length);
  }
};

#endif