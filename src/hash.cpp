#include "hash.hpp"
#include <boost/container_hash/hash.hpp>

//Не нашел в boost альтернативные алгоритмы хеширования.

size_t hash(std::string_view view) {
    boost::hash<std::string_view> h;
    return h(view);
}