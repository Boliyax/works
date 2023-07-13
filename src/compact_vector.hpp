#ifndef COMPACT_VECTOR
#define COMPACT_VECTOR

#include <memory>

// Упрощенный контейнер наподобие обычного вектора, но аллацирующий память
// каждый раз при добавлении нового элемента, увеличивая свой объем ровно на
// один индекс. Доступ к элементам реализован через аллокаты, а добавление новых
// через emplace_back.
template <typename T, typename Allocator = std::allocator<T>>
class CompactVector {
  T *data;
  size_t _size;
  Allocator allocator;

  template <typename TP>
  class Iterator : public std::iterator<std::random_access_iterator_tag, TP> {
    TP *p;

  public:
    using value_type = TP;
    using pointer = TP *;
    using reference = TP &;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = ptrdiff_t;

    Iterator() : p(nullptr) {}
    Iterator(pointer pt) : p(pt) {}

    Iterator &operator++() {
      ++p;
      return *this;
    }

    Iterator operator++(int) {
      iterator tmp(*this);
      operator++();
      return tmp;
    }

    Iterator &operator--() {
      --p;
      return *this;
    }

    Iterator operator--(int) {
      iterator tmp(*this);
      operator--();
      return tmp;
    }

    inline reference operator*() const noexcept { return *p; }
    inline pointer operator->() const noexcept { return p; }

    inline bool operator==(const Iterator &other) const noexcept {
      return p == other.p;
    }
    inline bool operator!=(const Iterator &other) const noexcept {
      return !(*this == other);
    }

    inline difference_type operator-(const Iterator &value) const noexcept {
      return p - value.p;
    }

    inline Iterator operator-(const difference_type value) const noexcept {
      return Iterator(p - value);
    }

    inline Iterator operator+(const difference_type value) const noexcept {
      return Iterator(p + value);
    }

    inline bool operator<(const Iterator &other) const noexcept {
      return p < other.p;
    }
  };

  inline void free(T *point) noexcept {
    for (size_type i = 0; i < _size; ++i) {
      std::allocator_traits<Allocator>::destroy(allocator, &(point[i]));
    }
  }

  inline void copy(const T *point) noexcept {
    for (size_type i = 0; i < _size; ++i) {
      std::allocator_traits<Allocator>::construct(allocator, &(data[i]),
                                                  point[i]);
    }
  }

  inline void move(T *point) noexcept {
    for (size_type i = 0; i < _size; ++i) {
      std::allocator_traits<Allocator>::construct(allocator, &(data[i]),
                                                  std::move(point[i]));
    }
  }

public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = Iterator<T>;
  using const_iterator = Iterator<const T>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  inline bool operator==(const CompactVector &other) const noexcept {
    return data == other.data;
  }
  inline bool operator!=(const CompactVector &other) const noexcept {
    return *this != other;
  }

  CompactVector() noexcept : data(nullptr), _size(0), allocator() {}

  CompactVector(const Allocator &alloc) noexcept
      : data(nullptr), _size(0), allocator(alloc) {}

  ~CompactVector() noexcept {
    free(data);
    std::allocator_traits<Allocator>::deallocate(allocator, data, _size);
  }

  CompactVector(const CompactVector &other)
      : data(
            std::allocator_traits<Allocator>::allocate(allocator, other._size)),
        _size(other._size), allocator(other.allocator) {
    copy(other.data);
  }

  CompactVector(CompactVector &&other)
      : data(other.data), _size(other._size), allocator(other.allocator) {
    other.data = nullptr;
    other._size = 0;
  }

  CompactVector &operator=(const CompactVector &other) {
    if (*this == other)
      return *this;
    this->~CompactVector();
    _size = other._size;
    allocator = other.allocator;
    data = std::allocator_traits<Allocator>::allocate(allocator, _size);
    copy(other.data);
    return *this;
  }

  CompactVector &operator=(CompactVector &&other) {
    if (*this == other)
      return *this;

    this->~CompactVector();
    _size = other._size;
    allocator = other.allocator;
    data = other.data;
    other.data = nullptr;
    other._size = 0;
    return *this;
  }

  template <typename... Types> CompactVector &emplace_back(Types &&...args) {
    T *old_ptr = data;
    data = std::allocator_traits<Allocator>::allocate(allocator, _size + 1);
    move(old_ptr);
    free(old_ptr);
    std::allocator_traits<Allocator>::deallocate(allocator, old_ptr, _size);
    std::allocator_traits<Allocator>::construct(allocator, &(data[_size]),
                                                std::forward<Types>(args)...);
    _size += 1;
    return *this;
  }

  inline size_type size() const noexcept { return _size; }
  inline bool empty() const noexcept { return _size == 0; }

  iterator begin() noexcept { return iterator(data); }
  reverse_iterator rbegin() noexcept { return reverse_iterator(data + _size); };
  const_iterator cbegin() const noexcept { return const_iterator(data); }
  const_iterator begin() const noexcept { return const_iterator(data); }
  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(data + _size);
  }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(data + _size);
  }

  iterator end() noexcept { return iterator(data + _size); }
  reverse_iterator rend() noexcept { return reverse_iterator(data); }
  const_iterator cend() const noexcept { return const_iterator(data + _size); }
  const_iterator end() const noexcept { return const_iterator(data + _size); }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(data);
  }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(data);
  }
};

#endif