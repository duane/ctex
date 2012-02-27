#include <Util/SmallIntMap.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(SmallIntMapTest, EmptyTest) {
  SmallIntMap<uint32_t, 8> empty;
  ASSERT_EQ(0u, empty.entries());
}

TEST(SmallIntMapTest, SetGet) {
  SmallIntMap<uint32_t, 8> map;
  for (uint32_t i = 0; i < 64; i++) {
    map.set(i, i);
  }
  ASSERT_EQ(64u, map.entries());
  for (uint32_t k = 0; k < 64; k++) {
    const uint32_t *v = map.get(k);
    ASSERT_TRUE(v != NULL);
    ASSERT_EQ(k, *v);
  }
  ASSERT_EQ(NULL, map.get(64));
}