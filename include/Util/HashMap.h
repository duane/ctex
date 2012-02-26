#ifndef __INCLUDE_UTIL_HASHMAP_H__
#define __INCLUDE_UTIL_HASHMAP_H__

#include <cassert>
#include <cstring>
#include <stdint.h>

namespace tex {

// a helper method, HashUInt.

class HashUInt {
public:
  uint32_t val;

  HashUInt(const uint32_t &v = 0) : val(v) {}

  uint32_t hash(void) const {
    return val;
  }

  bool operator==(const HashUInt &other) {
    return val == other.val;
  }

  HashUInt operator=(const uint32_t &v) {
    return HashUInt(v);
  }

};

template<typename K, typename V>
class HashMap {
private:
  HashMap &operator=(const HashMap &);
public:
  struct HashEntry {
    HashEntry *next;
    K key;
    V val;
  };

private:
  const uint32_t table_size;
  HashEntry **table;
  uint32_t table_entries;

private:
  // private utility methods
  HashEntry *find_entry(K &key, HashEntry ***reference) const {
    const uint32_t hash = key.hash();
    const uint32_t bucket = hash % table_size;
    HashEntry **prev = &table[bucket], *entry;
    while ((entry = *prev)) {
      if (key == entry->key) {
        *reference = prev;
        return entry;
      }
      prev = &entry->next;
    }
    // no luck finding our entry.
    *reference = prev;
    return NULL;
  }

public:
  HashMap(uint32_t size = 512) : table_size(size), table_entries(0) {
    table = new HashEntry*[table_size];
    memset(table, 0, table_size * sizeof(HashEntry*));
  }

  ~HashMap(void) {
    for (uint32_t i = 0; i < table_size; i++) {
      HashEntry *entry = table[i];
      while (entry) {
        HashEntry *next = entry->next;
        delete entry;
        entry = next;
      }
    }
    delete[] table;
  }

  uint32_t size(void) const {
    return table_size;
  }

  uint32_t entries(void) const {
    return table_entries;
  }

  void set(K &key, const V &value) {
    HashEntry **reference = NULL, *entry;
    entry = find_entry(key, &reference);
    assert(reference
    && "Internal error! Failed to find correct entry position in hash map.");
    if (entry) {
      entry->val = value;
    } else {
      // add an entry.
      table_entries += 1;
      entry = new HashEntry;
      entry->next = NULL;
      entry->key = key;
      entry->val = value;
      *reference = entry;
    }
  }

  V *get(K &key) const {
    HashEntry **reference, *entry;
    entry = find_entry(key, &reference);
    if (!entry)
      return NULL;
    return &entry->val;
  }

  HashEntry *get_entry(K &key) const {
    HashEntry **reference, *entry;
    entry = find_entry(key, &reference);
    return entry;
  }

public:
  class iterator {
  public:
    const HashMap<K, V> *map;
    uint32_t curr_bucket;
    HashEntry *curr_entry;

    bool operator==(const iterator &iter) const {
      return curr_bucket == iter.curr_bucket && curr_entry == iter.curr_entry;
    }

    bool operator!=(const iterator &iter) const {
      return curr_bucket != iter.curr_bucket || curr_entry != iter.curr_entry;
    }

    void operator++(int) {
      map->advance_iterator(*this);
    }

    K &key(void) const {
      assert(curr_entry && "Attempted to fetch key for NULL hash entry.");
      return curr_entry->key;
    }

    V &value(void) const {
      assert(curr_entry && "Attempted to fetch value for NULL hash entry.");
      return curr_entry->val;
    }

    iterator(const HashMap<K, V> *map_ref, uint32_t bucket, HashEntry *entry) :
             map(map_ref), curr_bucket(bucket), curr_entry(entry) {}

  };

  void advance_iterator(iterator &iter) const {
    iterator end_iter = end();
    assert(iter != end_iter &&
    "Attempted to advance end iterator.");
    if (iter.curr_entry)
      iter.curr_entry = iter.curr_entry->next;
    while (!iter.curr_entry) {
      iter.curr_bucket += 1;
      if (iter.curr_bucket >= table_size)
        return;
      iter.curr_entry = table[iter.curr_bucket];
    }
  }

public:
  iterator begin() const {
    if (table_entries == 0)
      return end();
    iterator iter = iterator(this, 0, table[0]);
    if (!iter.curr_entry)
      advance_iterator(iter);
    return iter;
  }

  iterator end() const {
    return iterator(this, table_size, NULL);
  }

// Now an explicit deep-copy constructor.
public:
  explicit HashMap(const HashMap &other) :
                   table_size(other.size()), table_entries(0) {
    table = new HashEntry*[table_size];
    memset(table, 0, table_size * sizeof(HashEntry*));

    for (iterator iter = other.begin(); iter != other.end(); iter++) {
      set(iter.key(), iter.value());
    }
  }

  /*HashMap operator=(const HashMap &other) const {
    return HashMap(other);
  }*/
};

}  // namespace tex

#endif  // __INCLUDE_UTIL_HASHMAP_H__