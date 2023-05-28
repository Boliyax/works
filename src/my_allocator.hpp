#ifndef MY_ALLOCATOR
#define MY_ALLOCATOR

#include "smart_bitset.hpp"
#include <array>

// Аллокаторы переменных одиннакового размера, и имеющие одинаковые размеры
// пулов используют общий для них менеджер контенеров (хранилищь).
template <size_t N, size_t POOL_SIZE> class StaticStoreManager {
  struct Store {
    using buffer_var = std::array<unsigned char, N>;
    SmartBitSet<POOL_SIZE> pool_manager;
    std::array<buffer_var, POOL_SIZE> pool;
    size_t counter = 0; // Счетчик занятых ячеек, если дополнительный контейнер
                        // окажется пуст - освободим занятую им память
    Store *nextpoint = nullptr; // При добавлении новых контейнеров, они
                                // выстраиваются в односвязный список
  };

  static Store primary_store;

public:
  static void *allocate(size_t n) {
    if (n == 0) {
      return nullptr;
    }
    if (n > POOL_SIZE) {
      return ::operator new(n *N);
    }

    Store *oldpoint = nullptr;
    // Перебор листа контейнеров, c поиском подходящего.
    for (auto *point = &primary_store; point != nullptr;
         point = point->nextpoint) {
      auto pos = point->pool_manager.acquire(n);
      if (pos < POOL_SIZE) {
        point->counter += n;
        return &(point->pool[pos]);
      }
      oldpoint = point;
    }

    // Если не нашли - выделяем новый.
    auto point = new Store();
    oldpoint->nextpoint = point;
    auto pos = point->pool_manager.acquire(n);
    point->counter += n;
    return &(point->pool[pos]);
  }

  static void deallocate(void *p, size_t n) {
    if (n > POOL_SIZE) {
      ::operator delete(p);
      return;
    }
    if (n == 0) {
      return;
    }
    Store *oldpoint = nullptr;
    // Перебор листа контейнеров, c поиском того что содержит переданный в метод
    // указатель.
    for (auto *point = &primary_store; point != nullptr;
         point = point->nextpoint) {
      if ((p >= &(point->pool[0])) && (p <= &(point->pool[POOL_SIZE - 1]))) {
        size_t e =
            static_cast<typename Store::buffer_var *>(p) - &(point->pool[0]);
        point->pool_manager.release(e, n);
        point->counter -= n;
        if ((oldpoint != nullptr) && (point->counter == 0)) {
          oldpoint->nextpoint = point->nextpoint;
          delete (point);
        }
        return;
      }
      oldpoint = point;
    }
  }
};

// Инициализация первичного (статического) контейнера для каждого менеджера
// контейнеров.
template <size_t N, size_t POOL_SIZE>
typename StaticStoreManager<N, POOL_SIZE>::Store
    StaticStoreManager<N, POOL_SIZE>::primary_store =
        StaticStoreManager<N, POOL_SIZE>::Store{};

// Класс для задания размеров пулов.
template <size_t LIMIT> struct LimitGroup {

  template <class T> class MyAllocator {
  public:
    using value_type = T;

    using manager = StaticStoreManager<sizeof(T), LIMIT>;

    MyAllocator() noexcept {}
    template <class U> MyAllocator(const MyAllocator<U> &) noexcept {}

    T *allocate(std::size_t n) {
      return static_cast<T *>(manager::allocate(n));
    }
    void deallocate(T *p, std::size_t n) { manager::deallocate(p, n); }
  };
};

template <class T, size_t LA, class U, size_t LB>
constexpr bool
operator==(const typename LimitGroup<LA>::MyAllocator<T> &a1,
           const typename LimitGroup<LB>::MyAllocator<U> &a2) noexcept {
  return true;
}

template <class T, size_t LA, class U, size_t LB>
constexpr bool
operator!=(const typename LimitGroup<LA>::MyAllocator<T> &a1,
           const typename LimitGroup<LB>::MyAllocator<U> &a2) noexcept {
  return false;
}

template <typename T, size_t LIMIT = 512>
using MyAllocator = typename LimitGroup<LIMIT>::MyAllocator<T>;

#endif