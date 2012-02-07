#ifndef __INCLUDE_UTIL_UNIQUEPTR_H__
#define __INCLUDE_UTIL_UNIQUEPTR_H__

#include <assert.h>

namespace tex {
template<typename T>
class UniquePtr {
private:
  UniquePtr(const UniquePtr&);
  UniquePtr &operator=(const UniquePtr&);
  T *ptr;
public:
  explicit UniquePtr(T *p = NULL) : ptr(p) {}
  
  T &operator*() const {
    assert(ptr && "Dereferencing null pointer!");
    return *ptr;
  }
  
  // member operator for convenience.
  T *operator->() const {return ptr;}
  
  operator bool() const {
    return ptr != NULL;
  }
  
  bool operator!() const {
    return ptr == NULL;
  }
  
  void reset(T *p = NULL) {
    delete ptr;
    ptr = p;
  }
  
  T *get() const {
    return ptr;
  }
  
  T *take() {
    T *p = ptr;
    ptr = NULL;
    return p;
  }
  
  ~UniquePtr() {
    delete ptr;
  }
};

}  // namespace tex
#endif  // __INCLUDE_UTIL_UNIQUEPTR_H__