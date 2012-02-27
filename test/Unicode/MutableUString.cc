#include <Unicode/UString.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(MutableUStringTest, EmptyInit) {
  MutableUString string;
  ASSERT_EQ(string.get_length(), 0u);
  ASSERT_GT(string.get_allocated(), 0u);
}

TEST(MutableUStringTest, UStringCopy) {
  UString string = UString("Hello");
  MutableUString mut_string = MutableUString(string);
  ASSERT_TRUE(mut_string == string);
  ASSERT_TRUE(mut_string == string);
  ASSERT_EQ(string.get_length(), mut_string.get_length());
  ASSERT_EQ(string.hash(), mut_string.hash());
}

TEST(MutableUStringTest, Add) {
  MutableUString string;
  ASSERT_EQ(string.get_length(), 0u);
  string.add((unichar)'Q');
  ASSERT_EQ(string.get_length(), 1u);
  ASSERT_EQ(string.get(0), (unichar)'Q');
}

TEST(MutableUStringTest, Set) {
  MutableUString string;
  string.add((unichar)'Q');
  ASSERT_EQ(string.get_length(), 1u);
  string.set(0, (unichar)'X');
  ASSERT_EQ(string.get_length(), 1u);
  ASSERT_EQ(string.get(0), (unichar)'X');
}