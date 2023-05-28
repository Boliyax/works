#include "my_allocator.hpp"
#include "compact_vector.hpp"

#include <iostream>
#include <map>

int factorial(int n) {
    int result = 1;
    for(int i = 2; i <=n; ++i) {
        result *= i;
    }
    return result;
}
    constexpr int RANGE = 10;
int main() {

    std::map<int, int> std_map;
    for(int i = 0; i < RANGE; ++i) {
        std_map.emplace(std::pair(i, factorial(i)));
    }

    std::map<int, int, std::less<int>, MyAllocator<std::pair<int, int>, RANGE>> my_alloc_map;
    for(int i = 0; i < RANGE; ++i) {
        my_alloc_map.emplace(std::pair(i, factorial(i)));
    }

    std::cout << "std::allocator map:";
    for(auto& i: std_map) {
        std::cout << " (" << i.first << ' ' << i.second << ')';
    }
    std::cout << std::endl;

    std::cout << "MyAllocator map:";
    for(auto& i: my_alloc_map) {
        std::cout << " (" << i.first << ' ' << i.second << ')';
    }
    std::cout << std::endl;

    CompactVector<int> std_c_vec;
    for(int i = 0; i < RANGE; ++i) {
        std_c_vec.emplace_back(i);
    }

    CompactVector<int, MyAllocator<int, RANGE>> my_alloc_c_vec;
    for(int i = 0; i < RANGE; ++i) {
        my_alloc_c_vec.emplace_back(i);
    }

    std::cout << "std::allocator CompactVector:";
    for(auto& i: std_c_vec) {
        std::cout << ' ' << i ;
    }
    std::cout << std::endl;

    std::cout << "MyAllocator CompactVector:";
    for(auto& i: my_alloc_c_vec) {
        std::cout << ' ' << i ;
    }
    std::cout << std::endl;
}
