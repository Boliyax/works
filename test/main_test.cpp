#include "my_allocator.hpp"
#include "smart_bitset.hpp"
#include "compact_vector.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>



constexpr uint32_t operator""_ui32(unsigned long long literal) noexcept {
  return literal;
}

TEST(smart_bitset, test1) {
  SmartBitSet<512> myset;
  auto a = myset.acquire(50);
  ASSERT_EQ(a, 0);
  a = myset.acquire(100);
  ASSERT_EQ(a, 50);
  a = myset.acquire(100);
  ASSERT_EQ(a, 150);
  a = myset.acquire(10);
  ASSERT_EQ(a, 250);
  a = myset.acquire(240);
  ASSERT_EQ(a, 260);
  myset.release(0, 50);
  a = myset.acquire(12);
  ASSERT_EQ(a, 0);
  a = myset.acquire(39);
  ASSERT_EQ(a, 512);
  myset.release(50, 100);
  myset.release(250, 10);
  a = myset.acquire(137);
  ASSERT_EQ(a, 12);
  a = myset.acquire(12);
  ASSERT_EQ(a, 500);
  a = myset.acquire(1);
  ASSERT_EQ(a, 149);
  a = myset.acquire(10);
  ASSERT_EQ(a, 250); 
}

TEST(my_allocator, map_and_vector) {
  using my_map = std::map<uint32_t, uint32_t, std::less<uint32_t>,
                          MyAllocator<std::pair<const uint32_t, uint32_t>>>;
  my_map map;
  map.emplace(std::make_pair(34_ui32, 65_ui32));
  map.emplace(std::make_pair(1_ui32, 82_ui32));
  map.emplace(std::make_pair(8_ui32, 90_ui32));
  map.emplace(std::make_pair(13_ui32, 62_ui32));
  map.emplace(std::make_pair(4_ui32, 9_ui32));
  map.emplace(std::make_pair(27_ui32, 1_ui32));

  std::vector<uint32_t, MyAllocator<uint32_t>> vector;
  vector.emplace_back(82);
  vector.emplace_back(9);
  vector.emplace_back(90);
  vector.emplace_back(62);
  vector.emplace_back(1);
  vector.emplace_back(65);

  auto veci = vector.begin();
  for (auto &i : vector) {
    EXPECT_EQ(i, *veci++);
  }
}

TEST(my_allocator, limit) {
  std::vector<int, MyAllocator<int, 25>> vec1;
  std::vector<int> vec2;

  vec1.emplace_back(5);
  vec1.emplace_back(10);
  vec1.emplace_back(15);
  vec1.emplace_back(20);
  vec1.emplace_back(25);

  vec2.emplace_back(5);
  vec2.emplace_back(10);
  vec2.emplace_back(15);
  vec2.emplace_back(20);
  vec2.emplace_back(25);

  EXPECT_TRUE(std::equal(vec1.cbegin(), vec1.cend(), vec2.cbegin(), vec2.cend()));
}

TEST(compact_vector, std_alloc) {
  CompactVector<uint32_t> my_vec;
  std::vector<uint32_t> std_vec;

  my_vec.emplace_back(54);
  my_vec.emplace_back(12);
  my_vec.emplace_back(14);
  my_vec.emplace_back(32);
  my_vec.emplace_back(83);

  std_vec.emplace_back(54);
  std_vec.emplace_back(12);
  std_vec.emplace_back(14);
  std_vec.emplace_back(32);
  std_vec.emplace_back(83);

  ASSERT_NE(my_vec.begin(), my_vec.end() );
  ASSERT_NE(my_vec.rbegin(), my_vec.rend() );
  ASSERT_NE(my_vec.cbegin(), my_vec.cend() );
  ASSERT_NE(my_vec.crbegin(), my_vec.crend() );
  ASSERT_EQ(my_vec.size(), std_vec.size());

  EXPECT_TRUE(std::equal(my_vec.cbegin(), my_vec.cend(), std_vec.cbegin(), std_vec.cend()));
  EXPECT_TRUE(std::equal(my_vec.crbegin(), my_vec.crend(), std_vec.crbegin(), std_vec.crend()));
  EXPECT_TRUE(std::equal(my_vec.begin(), my_vec.end(), std_vec.begin(), std_vec.end()));
  EXPECT_TRUE(std::equal(my_vec.rbegin(), my_vec.rend(), std_vec.rbegin(), std_vec.rend()));

  EXPECT_EQ(*(std::max_element(my_vec.cbegin(), my_vec.cend())) , *(std::max_element(std_vec.cbegin(), std_vec.cend())));
  EXPECT_EQ(*(std::min_element(my_vec.cbegin(), my_vec.cend())) , *(std::min_element(std_vec.cbegin(), std_vec.cend())));

  std::sort(my_vec.begin(), my_vec.end());
  std::sort(std_vec.begin(), std_vec.end());

  EXPECT_TRUE(std::equal(my_vec.cbegin(), my_vec.cend(), std_vec.cbegin(), std_vec.cend()));
  EXPECT_TRUE(std::equal(my_vec.crbegin(), my_vec.crend(), std_vec.crbegin(), std_vec.crend()));
  EXPECT_TRUE(std::equal(my_vec.begin(), my_vec.end(), std_vec.begin(), std_vec.end()));
  EXPECT_TRUE(std::equal(my_vec.rbegin(), my_vec.rend(), std_vec.rbegin(), std_vec.rend()));
}

TEST(compact_vector, my_alloc) {
  CompactVector<uint32_t, MyAllocator<uint32_t> > my_vec;
  std::vector<uint32_t> std_vec;

  my_vec.emplace_back(54);
  my_vec.emplace_back(12);
  my_vec.emplace_back(14);
  my_vec.emplace_back(32);
  my_vec.emplace_back(83);

  std_vec.emplace_back(54);
  std_vec.emplace_back(12);
  std_vec.emplace_back(14);
  std_vec.emplace_back(32);
  std_vec.emplace_back(83);

  ASSERT_NE(my_vec.begin(), my_vec.end() );
  ASSERT_NE(my_vec.rbegin(), my_vec.rend() );
  ASSERT_NE(my_vec.cbegin(), my_vec.cend() );
  ASSERT_NE(my_vec.crbegin(), my_vec.crend() );
  ASSERT_EQ(my_vec.size(), std_vec.size());

  EXPECT_TRUE(std::equal(my_vec.cbegin(), my_vec.cend(), std_vec.cbegin(), std_vec.cend()));
  EXPECT_TRUE(std::equal(my_vec.crbegin(), my_vec.crend(), std_vec.crbegin(), std_vec.crend()));
  EXPECT_TRUE(std::equal(my_vec.begin(), my_vec.end(), std_vec.begin(), std_vec.end()));
  EXPECT_TRUE(std::equal(my_vec.rbegin(), my_vec.rend(), std_vec.rbegin(), std_vec.rend()));

  EXPECT_EQ(*(std::max_element(my_vec.cbegin(), my_vec.cend())) , *(std::max_element(std_vec.cbegin(), std_vec.cend())));
  EXPECT_EQ(*(std::min_element(my_vec.cbegin(), my_vec.cend())) , *(std::min_element(std_vec.cbegin(), std_vec.cend())));

  std::sort(my_vec.begin(), my_vec.end());
  std::sort(std_vec.begin(), std_vec.end());

  EXPECT_TRUE(std::equal(my_vec.cbegin(), my_vec.cend(), std_vec.cbegin(), std_vec.cend()));
  EXPECT_TRUE(std::equal(my_vec.crbegin(), my_vec.crend(), std_vec.crbegin(), std_vec.crend()));
  EXPECT_TRUE(std::equal(my_vec.begin(), my_vec.end(), std_vec.begin(), std_vec.end()));
  EXPECT_TRUE(std::equal(my_vec.rbegin(), my_vec.rend(), std_vec.rbegin(), std_vec.rend()));
}

TEST(compact_vector, copy_and_move) {
  
  CompactVector<uint64_t, MyAllocator<uint64_t> > my_vec;
  my_vec.emplace_back(54);
  my_vec.emplace_back(12);
  my_vec.emplace_back(14);
  my_vec.emplace_back(32);
  my_vec.emplace_back(83);

  CompactVector<uint64_t, MyAllocator<uint64_t> > my_vec2;
  my_vec2 = my_vec;
  CompactVector<uint64_t, MyAllocator<uint64_t> > my_vec3(my_vec);

  EXPECT_TRUE(std::equal(my_vec.cbegin(), my_vec.cend(), my_vec2.cbegin(), my_vec2.cend()));
  EXPECT_TRUE(std::equal(my_vec.cbegin(), my_vec.cend(), my_vec3.cbegin(), my_vec3.cend()));

  CompactVector<uint64_t, MyAllocator<uint64_t> > my_vec4;
  my_vec4 = std::move(my_vec2);
  CompactVector<uint64_t, MyAllocator<uint64_t> > my_vec5(std::move(my_vec3));

  EXPECT_TRUE(std::equal(my_vec.cbegin(), my_vec.cend(), my_vec4.cbegin(), my_vec4.cend()));
  EXPECT_TRUE(std::equal(my_vec.cbegin(), my_vec.cend(), my_vec5.cbegin(), my_vec5.cend()));

  EXPECT_TRUE(my_vec2.empty());
  EXPECT_TRUE(my_vec3.empty());

}