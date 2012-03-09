/*****************************************************************************
*  Copyright (c) 2012 Duane Ryan Bailey                                      *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*****************************************************************************/

#ifndef __INCLUDE_UTIL_SMALLVECTOR_H__
#define __INCLUDE_UTIL_SMALLVECTOR_H__

#include <cassert>
#include <cstring> // size_t, memcpy

namespace tex {

/* Idea taken from LLVM, file "include/llvm/ADT/TinyVector.h"
 * Code entirely original, however, but ultimately much less sophisticated
 * than the original idea: all elements must be copyable.
 */

/**
 *  SmallVector is a small vector that is initially stack-allocated.
 *  SmallVector takes two type parameters, T and n.
 *  T is the type of the element being stored in the vector,
 *  and n in is the size of the stack allocation. If the number of elements
 *  exceeds n, then the vector is copied to memory and proceeds as a normal
 *  vector. T must have a valid copy/assign operator.
 *
 *  SmallVector also exposes push/pop functions and can act as a stack.
 */
template <class T, int n>
class SmallVector {
private:
  SmallVector(const SmallVector &);
  SmallVector &operator=(const SmallVector &);

private:
  T stack[n];
  T *heap;
  size_t allocated;
  size_t elems;

public:
  SmallVector(void) : heap(NULL), allocated(n), elems(0) {
  }

  ~SmallVector(void) {
    if (heap)
      delete[] heap;
  }

  /**
   *  Fetches the number of elements in the vector.
   *  @return The number of elements in the vector.
   */
  size_t entries(void) const {
    return elems;
  }

  /**
   *  Appends element T to vector.
   *  @param elem The element to be added.
   */
  void append(T &elem) {
    /* get this assertion out of the way.
     * If allocated < n, which it never should be,
     * then the logic gets a lot more hairy.
     * So let's avoid it all together.
     */
    assert(allocated >= n && "Invalid allocation value.");
    if (elems >= allocated) {
      T *raw = new T[allocated * 2];
      if (allocated == n) { // we're switching from stack allocation
        for (size_t i = 0; i < elems; i++) {
          raw[i] = stack[i];
        }
      } else {
        for (size_t i = 0; i < elems; i++) {
          raw[i] = heap[i];
        }
        delete[] heap;
      }
      heap = raw;
      allocated *= 2;
    }

    if (allocated <= n)
      stack[elems] = elem;
    else
      heap[elems] = elem;
    elems += 1;
  }

  /**
   *  Fetches an element from the vector.
   *  @param i The index of the vector; must be less than the
   *  number of elements in the vector.
   *  @return A reference to the element in the vector.
   */
  T &get(size_t i) {
    assert(i < elems && "Attempted to access an element out of bounds.");
    if (allocated <= n)
      return stack[i];
    assert(heap && "Attempted to access NULL heap.");
    return heap[i];
  }

  void push(T &elem) {
    append(elem);
  }

  T pop(void) {
    elems -= 1;
    return stack[elems];
  }

  T &head(void) {
    return stack[elems - 1];
  }
};

}  // namespace tex

#endif  // __INCLUDE_UTIL_SMALLVECTOR_H__