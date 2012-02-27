#include <Util/HashMap.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(HashMapTest, Empty) {
  HashMap<HashUInt, HashUInt> map;
  HashUInt key = HashUInt(0);
  ASSERT_EQ(0u, map.entries());
  ASSERT_EQ((HashUInt*)NULL, map.get(key));
}

TEST(HashMapTest, SetGet) {
  HashMap<HashUInt, HashUInt> map;
  HashUInt key = HashUInt(5), val = HashUInt(42);
  ASSERT_EQ(0u, map.entries());
  ASSERT_EQ(NULL, map.get(key));
  map.set(key, val);
  HashUInt *gotten = map.get(key);
  ASSERT_NE((HashUInt*)NULL, gotten);
  ASSERT_TRUE(val == *gotten);
}

TEST(HashMapTest, Copy) {
  HashMap<HashUInt, uint32_t> map(8);
  ASSERT_EQ(0u, map.entries());
  ASSERT_EQ(8u, map.size());
  HashMap<HashUInt, uint32_t> copy(map);
  ASSERT_EQ(0u, copy.entries());
  ASSERT_EQ(8u, copy.size());
}

TEST(HashMapTest, FullCopy) {
  HashMap<HashUInt, uint32_t> map(8);
  for (uint32_t i = 0; i < 64; i++) {
    HashUInt key = i;
    map.set(key, i);
  }
  ASSERT_EQ(64u, map.entries());
  HashMap<HashUInt, uint32_t> copy(map);
  ASSERT_EQ(map.entries(), copy.entries());
  ASSERT_EQ(map.size(), copy.size());
}
