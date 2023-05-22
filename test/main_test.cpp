#include "ip.hpp"
#include "split.hpp"
#include <gtest/gtest.h>

TEST(Ip, string_parse) {
  Ip ip("54.80.16.2");
  ASSERT_EQ(ip.addres[0], 54);
  ASSERT_EQ(ip.addres[1], 80);
  ASSERT_EQ(ip.addres[2], 16);
  ASSERT_EQ(ip.addres[3], 2);
}

TEST(Ip, string_cast) {
  Ip ip("54.80.16.2");
  ASSERT_EQ(static_cast<const std::string>(ip), "54.80.16.2");
}

TEST(Split, tab_separator) {
  auto vec = split("one\ttwo\tthree", '\t');
  ASSERT_EQ(vec.size(), 3);
  ASSERT_EQ(vec[0], "one");
  ASSERT_EQ(vec[1], "two");
  ASSERT_EQ(vec[2], "three");
}

TEST(Split, dot_separator) {
  auto vec = split("one.two.three", '.');
  ASSERT_EQ(vec.size(), 3);
  ASSERT_EQ(vec[0], "one");
  ASSERT_EQ(vec[1], "two");
  ASSERT_EQ(vec[2], "three");
}