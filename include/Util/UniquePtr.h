#ifndef __INCLUDE_UTIL_UNIQUEPTR_H__
#define __INCLUDE_UTIL_UNIQUEPTR_H__

#include <cassert>

namespace tex {

/**
 *  UniquePtr is a simple wrapper around a traditional pointer that has several benefits.
 *  1. It cannot be copied or assigned without explicitely doing so, forcing the programmer to be careful about considering memory management.
 *  2. It deletes the given pointer on its deconstructor and in reset(). Because UniquePtr is intended to be passed around by reference and never by pointer, this allows tying the life of a pointer to a given scope.
 *  3. Access to the wrapped object is allowed through the member operator.
 */ 
template<typename T>
class UniquePtr {
private:
  UniquePtr(const UniquePtr&);
  UniquePtr &operator=(const UniquePtr&);
  T *ptr;
public:
  /** Initializes the UniquePtr with given ptr. */
  explicit UniquePtr(T *p = NULL) : ptr(p) {}
  
  /** Dereferences wrapped pointer. */
  T &operator*() const {
    assert(ptr && "Dereferencing null pointer!");
    return *ptr;
  }
  
  /** Member operator for convenience. */
  T *operator->() const {
    assert(ptr && "Accessing member of null pointer!");
    return ptr;
  }
  
  /** The same as calling bool(ptr). */
  operator bool() const {
    return ptr != NULL;
  }
  
  /** The opposite of the bool() operator. */
  bool operator!() const {
    return ptr == NULL;
  }
  
  /** Deletes the former pointer and acquires the new pointer. */
  void reset(T *p = NULL) {
    delete ptr;
    ptr = p;
  }
  
  /** Fetches the pointer without releasing it. */
  T *get() const {
    return ptr;
  }
  
  /** Returns the pointer and sets the internal pointer to NULL. */
  T *take() {
    T *p = ptr;
    ptr = NULL;
    return p;
  }
  
  /** Deletes wrapped pointer. */
  ~UniquePtr() {
    delete ptr;
  }
};

}  // namespace tex
#endif  // __INCLUDE_UTIL_UNIQUEPTR_H__