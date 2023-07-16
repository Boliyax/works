#include <array>
#include <map>
#include <memory>


template <typename T, T DEFAULT, size_t N> class Matrix {
public:
  using Key = std::array<size_t, N>;
  using Container = std::map<Key, T>;

private:
  std::shared_ptr<Container> map = std::make_shared<Container>();

public:
  class Reference {
    friend class Matrix<T, DEFAULT, N>;

  public:
    operator T() {
      if (container->contains(key)) {
        return (*container)[key];
      }
      return DEFAULT;
    }
    Reference &operator=(const T &object) {
      (*container)[key] = object;
      if ((*container)[key] == DEFAULT)
        container->erase(key);
      return *this;
    }
    Reference &operator=(T &&object) {
      (*container)[key] = std::move(object);
      if ((*container)[key] == DEFAULT)
        container->erase(key);
      return *this;
    }

  private:
    Reference() = default;
    Reference(std::shared_ptr<Container> map, size_t index)
        : container(map), key{index} {}
    std::shared_ptr<Container> container;
    Key key = Key();
  };

  class Iterator {
    Container::iterator iter;

  public:
    Iterator(Container::iterator i): iter(i) {}
    bool operator==(Iterator i) { return iter == i.iter; }
    bool operator!=(Iterator i) { return !(*this == i); }
    Iterator &operator++() {
      ++iter;
      return *this;
    }
    Iterator &operator--() {
      --iter;
      return *this;
    }
    Iterator operator++(int dummy) {
      auto temp = *this;
      ++iter;
      return temp;
    }
    Iterator operator--(int dummy) {
      auto temp = *this;
      --iter;
      return temp;
    }

  private:
    template <std::size_t... Is>
    auto unpack_impl(std::index_sequence<Is...>)
        -> decltype(std::make_tuple(iter->first[Is]..., iter->second)) {
      return std::make_tuple(iter->first[Is]..., iter->second);
    }

    auto unpack() -> decltype(unpack_impl(std::make_index_sequence<N>{})) {
      return unpack_impl(std::make_index_sequence<N>{});
    }

  public:
    auto operator*() -> decltype(this->unpack()) { return unpack(); }
  };

private:
  template <size_t DEPTH, bool CRUTCH = true> struct Node {
    void init_ref(Reference &reference) {
      reference.key[DEPTH - 1] = index;
      parent->init_ref(reference);
    }
    Node(const std::shared_ptr<Node<DEPTH - 1>> &p, size_t i)
        : parent(p), index(i) {}
    std::shared_ptr<Node<DEPTH - 1>> parent;
    size_t index;
  };

  template <bool CRUTCH> struct Node<1, CRUTCH> {
    void init_ref(Reference &reference) {
      reference.key[0] = index;
      reference.container = container;
    }
    Node(const std::shared_ptr<Container> &c, size_t i)
        : container(c), index(i) {}
    std::shared_ptr<Container> container;
    size_t index;
  };

public:
  size_t size() { return map->size(); }

  template <size_t DEPTH, bool CRUTCH = true> class Level {
    friend class Matrix<T, DEFAULT, N>;

  public:
    Level(const std::shared_ptr<Node<DEPTH - 1>> &parent, size_t index)
        : node(std::make_shared<Node<DEPTH>>(parent, index)) {}
    Level<DEPTH + 1> operator[](size_t index) {
      return Level<DEPTH + 1>(node, index);
    }

  private:
    std::shared_ptr<Node<DEPTH>> node;
  };

  template <bool CRUTCH> class Level<N - 1, CRUTCH> {
    friend class Matrix<T, DEFAULT, N>;

  public:
    Level(const std::shared_ptr<Node<N - 2>> &parent, size_t index)
        : node(std::make_shared<Node<N - 1>>(parent, index)) {}
    Reference operator[](size_t index) {
      Reference reference;
      reference.key[N - 1] = index;
      node->init_ref(reference);
      return reference;
    }

  private:
    std::shared_ptr<Node<N - 1>> node;
  };

  template <bool CRUTCH> class Level<1, CRUTCH> {
    friend class Matrix<T, DEFAULT, N>;

  public:
    Level(const std::shared_ptr<Container> &container, size_t index)
        : node(std::make_shared<Node<1>>(container, index)) {}
    Level<2> operator[](size_t index) { return Level<2>(node, index); }

  private:
    std::shared_ptr<Node<1>> node;
  };

  template <bool CRUTCH> class Level<0, CRUTCH> {
    friend class Matrix<T, DEFAULT, N>;

  public:
    Level(const std::shared_ptr<Container> &container, size_t index)
        : node(std::make_shared<Node<1>>(container, index)) {}
    Reference operator[](size_t index) {
      Reference reference;
      reference.key[1] = index;
      node->init_ref(reference);
      return reference;
    }

  private:
    std::shared_ptr<Node<1>> node;
  };

private:
  template <size_t L, bool CRUTCH = true> struct FirstLevel {
    using type = Level<1>;
  };

  template <bool CRUTCH> struct FirstLevel<1, CRUTCH> {
    using type = Reference;
  };

  template <bool CRUTCH> struct FirstLevel<2, CRUTCH> {
    using type = Level<0>;
  };

public:
  typename FirstLevel<N>::type operator[](size_t index) {
    return typename FirstLevel<N>::type(map, index);
  }

  Iterator begin() { return Iterator(map->begin()); }

  Iterator end() { return Iterator(map->end()); }
};