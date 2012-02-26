#ifndef __INCLUDE_UTIL_SMALLINTMAP_H__
#define __INCLUDE_UTIL_SMALLINTMAP_H__

#include <cstring>

#include <Util/HashMap.h>

namespace tex {

template <typename T, uint32_t N>
class SmallIntMap {
private:
  // disallow copy/assign.
  SmallIntMap &operator=(const SmallIntMap &);

private:
  T stack[N];
  bool defined[N];
  HashMap<HashUInt, T> map;

public:
  SmallIntMap(uint32_t map_size=512) : map(map_size) {
    memset(defined, 0, sizeof(defined) * sizeof(bool));
  }

  ~SmallIntMap(void) {}

  uint32_t size(void) const {
    return map.size();
  }

  void set(uint32_t key, const T &val) {
    if (key < N) {
      stack[key] = val;
      defined[key] = true;
    } else {
      HashUInt k = HashUInt(key);
      map.set(k, val);
    }
  }

  const T *get(uint32_t key) const {
    if (key < N) {
      if (defined[key])
        return &stack[key];
      return NULL;
    }
    HashUInt k = HashUInt(key);
    return map.get(k);
  }

public:
  typedef typename HashMap<HashUInt, T>::iterator map_iterator;
  class iterator {
  public:
    const SmallIntMap<T, N> &map;
    uint32_t curr_bucket;
    map_iterator curr_iter;

    iterator(uint32_t bucket, map_iterator &iter,
             const SmallIntMap<T, N> &map_ref) :
             map(map_ref), curr_bucket(bucket), curr_iter(iter) {}

    bool operator==(const iterator other) const {
      return curr_bucket = other.curr_bucket && curr_iter == other.curr_iter;
    }

    bool operator!=(const iterator other) const {
      return (curr_bucket != other.curr_bucket)
              || (curr_iter != other.curr_iter);
    }

    void operator++(int) {
      map.advance_iterator(*this);
    }

    uint32_t key(void) const {
      if (curr_bucket < N)
        return curr_bucket;
      return curr_iter.key().val;
    }

    const T &value(void) const {
      if (curr_bucket < N) {
        const T *val = map.get(curr_bucket);
        assert(val && "Attempted to get undefined value from iterator.");
        return *val;
      }
      return curr_iter.value();
    }
  };  // class iterator

  iterator begin(void) const {
    map_iterator map_iter = map.begin();
    iterator iter(0, map_iter, *this);
    if (!defined[0])
      advance_iterator(iter);
    return iter;
  }

  iterator end(void) const {
    map_iterator end_iter = map.end();
    return iterator(N, end_iter, *this);
  }

  void advance_iterator(iterator &iter) const {
    iterator end_iter = end();
    assert(iter != end_iter &&
    "Attempted to advance iterator already at end.");
    if (iter.curr_bucket < N) {
      while (!defined[iter.curr_bucket]) {
        iter.curr_bucket += 1;
        if (iter.curr_bucket >= N)
          break;
      }
    } else {
      iter.curr_iter++;
    }
  }

  explicit SmallIntMap(const SmallIntMap &other) :
                       map(other.size()) {
    memset(defined, 0, sizeof(defined) * sizeof(bool));
    for (iterator iter = other.begin(); iter != other.end(); iter++) {
      uint32_t key = iter.key();
      const T &value = iter.value();
      set(key, value);
    }
  }
};

}

#endif  // __INCLUDE_UTIL_SMALLINTMAP_H__