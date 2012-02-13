#include <Unicode/UString.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(UStringTest, EmptyInit) {
  UString string = UString();
  ASSERT_EQ(string.get_length(), 0u);
  ASSERT_EQ(string.get_allocated(), 0u);
}

TEST(UStringTest, SingleChar) {
  unichar uc = 0x03BB; // lambda
  UString string = UString(&uc, 1u);
  ASSERT_EQ(string.get_length(), 1u);
  ASSERT_EQ(string.get_allocated(), sizeof(unichar) * 1);
}

TEST(UStringTest, CStringInit) {
  UString string = UString("Hello");
  ASSERT_EQ(string.get_length(), 5u);
  ASSERT_EQ(string.get_allocated(), sizeof(unichar) * 5u);
  ASSERT_EQ(string.get(0), (unichar)'H');
  ASSERT_EQ(string.get(4), (unichar)'o');
}