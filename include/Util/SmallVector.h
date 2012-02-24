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
  SmallVector(void) : elems(0), allocated(n), heap(NULL) {}
  ~SmallVector(void) {
    if (heap)
      delete[] heap;
  }

  /**
   *  Fetches the number of elements in the vector.
   *  @return The number of elements in the vector.
   */
  size_t elements(void) const {
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
      T *old;
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
  T &get(size_t i) const {
    assert(i < elems && "Attempted to access an element out of bounds.");
    if (allocated < n)
      return stack[n];
    assert(heap && "Attempted to access NULL heap.");
    return heap[n];
  }
};

}  // namespace tex

#endif  // __INCLUDE_UTIL_SMALLVECTOR_H__