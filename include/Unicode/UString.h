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

#ifndef __INCLUDE_USTRING_H__
#define __INCLUDE_USTRING_H__

#include <Unicode/Unicode.h>
#include <stdio.h>

namespace tex {

/** 
 *  UString is a simple, immutable string class of wide (32-bit) characters.
 *  @see MutableUString
 */
class UString {
public:
  
  /**
   * The default initializer returns an empty string and allocates nothing.
   */
  UString() : raw(NULL), length(0), allocated(0), code(0), dirty_hash(true)  {
    
  }
  
  /**
   *  This constructor allocates memory for the buffer, copies the given unichars into it, and marks the hash code as inaccurate.
   *  @param uchar The buffer of characters to be copied.
   *  @param len The length of the buffer, in unichars, not bytes.
   */
  UString(unichar *uchar, size_t len) {
    assert(uchar && "Got passed NULL buffer in constructor.");
    allocated = length = len;
    raw = new unichar[length];
    memcpy(raw, uchar, length * sizeof(unichar));
    dirty_hash = true;
  }
  
  /**
   *  A simple utility method that allocates and returns a new UString after decoding from ASCII.
   *  @param cstr The C-string to be decoded and copied.
   *  @see ASCIICodec
   */
   UString(const char *cstr);
  
  /** Copy/assign operator, newly allocates and copies data. */
  UString &operator=(const UString &other) {
    assert(this != &other && "Attempted to copy string to itself.");

    if (raw)
      delete[] raw;
    
    allocated = length = other.get_length();
    raw = new unichar[allocated];
    memcpy(raw, other.get_raw(), allocated * sizeof(unichar));
    dirty_hash = true;
    
    return *this;
  }
  
  /** Copy constructor. */
  UString(const UString &other) {
    allocated = length = other.get_length();
    raw = new unichar[allocated];
    memcpy(raw, other.get_raw(), allocated * sizeof(unichar));
    dirty_hash = true;
  }
  
  /** The deconstructor frees allocated memory. */
  ~UString() {
    delete[] raw;
  }
  
  /**
   *  Simply fetches the i'th character in the string.
   *  @param i The index to be accessed; must be a valid index (i < length of string).
   *  @return The character at index i.
   */
  unichar get(size_t i) const {
    assert (i < length && "Accessed a character out of bounds.");
    return raw[i];
  }
  
  /** Returns the internal character buffer. */
  unichar *get_raw() const {
    return raw;
  }
  
  /** Returns the size of the allocated buffer, in bytes. */
  size_t get_allocated() const {
    return allocated * sizeof(unichar);
  }

  /** Returns the number of characters stored in the UString. */
  size_t get_length() const {
    return length;
  }
  
  /** 
   *  Computes the 32-bit hash of the string if necessary using FNV-1.
   *  @see http://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
   */
  uint32_t hash() {
    if (dirty_hash) {
      // constants found at http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
      const uint32_t FNV1_PRIME = 16777619;
      const uint32_t FNV1_OFFSET = 2166136261;

      code = FNV1_OFFSET;
      uint8_t *bytes = (uint8_t*)raw;
      for (size_t i = 0; i < length * sizeof(unichar); i++) {
        uint8_t byte = bytes[i];
        code = code * FNV1_PRIME;
        code = code ^ byte;
      }
    }
    return code;
  }
  
  /**
   *  Compares the equality of the two strings.
   *  Checks equality using the following methods in order:
   *  1. a.length != b.length => false
   *  2. a.length == b.length == 0 => true
   *  3. a.hash != b.hash => false
   *  4. A character-by-character check.
   *  @param other The string to be compared.
   *  @return true on equal, false on unequal.
   */
  bool equalq(UString &other) {
    if (length != other.get_length())
      return false;
    
    if (length == 0 && other.get_length() == 0)
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
  /** The buffer of characters. */
  unichar *raw;
  /** The number of characters currently in the string. */
  size_t length;

  /** The size of the allocated buffer in characters, NOT bytes. **/
  size_t allocated;
  
  /** The memoized hash code. */
  uint32_t code;
  
  /** a flag marking an inaccurate hash code. */
  bool dirty_hash;
};

/**
 *  MutableUString is the mutable variant of UString.
 *  MutableUString is treated like a vector of bytes:
 *  1. The initial allocation for an empty string is 16 characters.
 *  2. If a character is added which should lie outside the allocated buffer, the allocated size is doubled and the string is copied.
 *  3. Unlike UString, the allocated buffer is often larger than the buffer being used; however, MutableUString can be passed anywhere a UString is requested.
 */
class MutableUString : public UString {
public:
  
  /** The default initializer. Initializes to 16 characters (64 bytes) allocated by default. */
  MutableUString() {
    allocated = 16;
    raw = new unichar[allocated];
    length = 0;
    dirty_hash = true;
  }
  
  /** The copy constructor; copies as expected. */
  explicit MutableUString(UString &string) {
    // copy the data
    allocated = length = string.get_length();
    raw = new unichar[allocated];
    memcpy(raw, string.get_raw(), allocated * sizeof(unichar));
    dirty_hash = true;
  }
  
  /**
   *  Initializes an empty MutableUString with the given allocation.
   *  @param initialAllocation The initial allocation in unichars.
   */
  explicit MutableUString(size_t initialAllocation) {
    allocated = initialAllocation;
    raw = new unichar[allocated];
    length = 0;
    dirty_hash = true;
  }
  
  /**
   *  Sets the character at index to given character.
   *  @param index The index of the character to be mutated. Must be an index less than the length of the string.
   *  @param uchar The new character to be placed at index.
   */
  void set(size_t index, unichar uchar) {
    assert(index < length && "Attempted to set character out of bounds.");
    raw[index] = uchar;
    dirty_hash = true;
  }
  
  /**
   *  Appends a character to the end of the string. If necessary, the string doubles its allocated buffer and copies the old to the new.
   *  @param uchar The character to be appended to the string.
   */
  void add(unichar uchar) {
    if (length >= allocated) {
      unichar *temp = new unichar[allocated * 2];
      memcpy(temp, raw, allocated * sizeof(unichar));
      delete[] raw;
      raw = temp;
      allocated = allocated * 2;
    }
    raw[length] = uchar;
    length += 1;
    dirty_hash = true;
  }
};

};  // namespace tex

#endif  // __INCLUDE_USTRING_H__
