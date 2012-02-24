#ifndef __INCLUDE_UTIL_HASHMAP_H__
#define __INCLUDE_UTIL_HASHMAP_H__

#include <cassert>
#include <cstring>
#include <stdint.h>

namespace tex {

template<typename K, typename V>
class HashMap {
private:
  // Disallow copy/assign.
  HashMap(const HashMap &);
  HashMap &operator=(const HashMap &);

private:
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

  uint32_t entries(void) const {
    return table_entries;
  }

  void set(K &key, V &value) {
    HashEntry **reference = NULL, *entry;
    entry = find_entry(key, &reference);
    assert(reference && "Internal error! Failed to find correct entry position in hash map.");
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
};

}

#endif  // __INCLUDE_UTIL_HASHMAP_H__