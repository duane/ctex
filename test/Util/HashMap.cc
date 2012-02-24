#include <Util/HashMap.h>
#include "gtest/gtest.h"

using namespace tex;

class HashInt {
public:
  uint32_t val;

  HashInt() : val(0) {}
  HashInt(uint32_t int_val) : val(int_val) {}

  uint32_t hash(void) const {
    return val;
  }

  bool operator==(const HashInt &other) const {
    return val == other.val;
  }
};

TEST(HashMapTest, Empty) {
  HashMap<HashInt, HashInt> map;
  HashInt key = HashInt(0);
  //ASSERT_EQ(0u, map.entries());
  //ASSERT_EQ((HashInt*)NULL, map.get(key));
}

TEST(HashMapTest, SetGet) {
  HashMap<HashInt, HashInt> map;
  HashInt key = HashInt(5), val = HashInt(42);
  ASSERT_EQ(0u, map.entries());
  ASSERT_EQ(NULL, map.get(key));
  map.set(key, val);
  HashInt *gotten = map.get(key);
  ASSERT_NE((HashInt*)NULL, gotten);
  ASSERT_EQ(val, *gotten);
}