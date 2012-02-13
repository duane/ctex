#include <Unicode/UString.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(MutableUStringTest, EmptyInit) {
  MutableUString string = MutableUString();
  ASSERT_EQ(string.getLength(), 0u);
  ASSERT_GT(string.getAllocated(), 0u);
}

TEST(MutableUStringTest, UStringCopy) {
  UString string = UString("Hello");
  MutableUString mut_string = MutableUString(string);
  ASSERT_TRUE(string.equalq(mut_string));
  ASSERT_TRUE(mut_string.equalq(string));
  ASSERT_EQ(string.getLength(), mut_string.getLength());
  ASSERT_EQ(string.hash(), mut_string.hash());
}

TEST(MutableUStringTest, Add) {
  MutableUString string = MutableUString();
  ASSERT_EQ(string.getLength(), 0u);
  string.add((unichar)'Q');
  ASSERT_EQ(string.getLength(), 1u);
  ASSERT_EQ(string.get(0), (unichar)'Q');
}

TEST(MutableUStringTest, Set) {
  MutableUString string = MutableUString();
  string.add((unichar)'Q');
  ASSERT_EQ(string.getLength(), 1u);
  string.set(0, (unichar)'X');
  ASSERT_EQ(string.getLength(), 1u);
  ASSERT_EQ(string.get(0), (unichar)'X');
}