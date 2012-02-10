/*
  Copyright 2010 Duane R. Bailey
  
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
  
    http://www.apache.org/licenses/LICENSE-2.0
  
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

/*
  UString is a standard, immutable string of unichars.
*/

#ifndef __INCLUDE_USTRING_H__
#define __INCLUDE_USTRING_H__

#include <Unicode/Unicode.h>
#include <stdio.h>

namespace tex {

class UString {
public:
  UString() : raw(NULL), length(0), size(0), allocated(0), code(0), dirty_hash(true)  {
    
  }
  UString(unichar *uchar, size_t len) {
    assert(uchar != NULL);
    length = len;
    raw = new unichar[length];
    memcpy(raw, uchar, length * sizeof(unichar));
    dirty_hash = true;
  }
  
  static UString *FromCString(const char *cstr);
  
  ~UString() {
    delete raw;
  }

  unichar get(size_t i) const {
    if (i >= length)
      return 0x0;
    return raw[i];
  }

  unichar *getRaw() const {
    return raw;
  }
  
  size_t getAllocated() const {
    return allocated * sizeof(unichar);
  }

  size_t getLength() const {
    return length;
  }
  
  uint32_t hash() {
    if (dirty_hash) {
      // FNV-1 hash, see http://en.wikipedia.org/wiki/Fowler_Noll_Vo_hash
      // and http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
      const uint32_t FNV1_PRIME = 16777619;
      const uint32_t FNV1_OFFSET = 2166136261;

      code = FNV1_OFFSET;
      uint8_t *bytes = (uint8_t*)raw;
      for (size_t i = 0; i < length * sizeof(unichar); i++) {
        uint8_t byte = bytes[i];
        code = code ^ byte;
        code = code * FNV1_PRIME;
      }
    }
    return code;
  }
  
  bool equalq(UString &other) {
    if (length != other.getLength())
      return false;
    
    if (length == 0 && other.getLength() == 0)
      return true;

    if (hash() != other.hash())
      return false;
    
    // now do a char-basis equality check.
    for (size_t i = 0; i < length; i++) {
      if (raw[i] != other.get(i))
        return false;
    }
    return true;
  }
    
protected: // So MutableUString can access them.
  unichar *raw;
  size_t length, size, allocated;
  uint32_t code;
  bool dirty_hash;
};

class MutableUString : public UString {
public:
  MutableUString() {
    allocated = 16;
    raw = new unichar[allocated];
    length = 0;
    dirty_hash = true;
  }
  explicit MutableUString(UString *string) {
    // copy the data
    allocated = length = string->getLength();
    raw = new unichar[length];
    memcpy(raw, string->getRaw(), allocated * sizeof(unichar));
    dirty_hash = true;
  }
  
  explicit MutableUString(size_t initialAllocation) {
    allocated = initialAllocation;
    raw = new unichar[allocated];
    length = 0;
    dirty_hash = true;
  }
     
  void set(size_t index, unichar uchar) {
    if (index >= length)
      return;
    raw[index] = uchar;
    dirty_hash = true;
  }
  
  void add(unichar uchar) {
    if (length >= allocated) {
      unichar *temp = new unichar[allocated * 2];
      memcpy(temp, raw, allocated * sizeof(unichar));
      delete raw;
      raw = temp;
      allocated = allocated * 2;
    }
    raw[length++] = uchar;
    dirty_hash = true;
  }
};

};  // namespace tex

#endif  // __INCLUDE_USTRING_H__
