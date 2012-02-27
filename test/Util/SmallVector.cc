#include <Util/SmallVector.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(SmallVectorTest, EmptyTest) {
  SmallVector<uint32_t, 16> vector;
  ASSERT_EQ(0u, vector.entries());
}

TEST(SmallVectorTest, AppendGet) {
  SmallVector<uint32_t, 16> vector;
  ASSERT_EQ(0u, vector.entries());
  uint32_t val = 42u;
  vector.append(val);
  ASSERT_EQ(1u, vector.entries());
  ASSERT_EQ(val, vector.get(0));
}

TEST(SmallVectorTest, ExpandBeyondBoundsTest) {
  SmallVector<uint32_t, 2> vector;
  ASSERT_EQ(0u, vector.entries());
  uint32_t val = 0;
  vector.append(val);
  val++;
  vector.append(val);
  val++;
  ASSERT_EQ(2u, vector.entries());
  ASSERT_EQ(0u, vector.get(0));
  ASSERT_EQ(1u, vector.get(1));
  vector.append(val);
  val++;
  vector.append(val);
  val++;
  ASSERT_EQ(4u, vector.entries());
  ASSERT_EQ(0u, vector.get(0));
  ASSERT_EQ(1u, vector.get(1));
  ASSERT_EQ(2u, vector.get(2));
  ASSERT_EQ(3u, vector.get(3));
  vector.append(val);
  val++;
  vector.append(val);
  val++;
  ASSERT_EQ(6u, vector.entries());
  ASSERT_EQ(0u, vector.get(0));
  ASSERT_EQ(1u, vector.get(1));
  ASSERT_EQ(2u, vector.get(2));
  ASSERT_EQ(3u, vector.get(3));
  ASSERT_EQ(4u, vector.get(4));
  ASSERT_EQ(5u, vector.get(5));
}