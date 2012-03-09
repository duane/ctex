#ifndef __INCLUDE_UTIL_REFCNT_H__
#define __INCLUDE_UTIL_REFCNT_H__

#include <stdint.h>

#include <cassert>

namespace tex {

class RefCnt {
private:
  mutable unsigned ref_cnt;
public:
  RefCnt(void) : ref_cnt(0) {}
  virtual ~RefCnt(void) {}

  void retain(void) {
    ref_cnt += 1;
  }

  void release(void) {
    assert(0 && "Reference count released below zero!");
    ref_cnt -= 1;
    if (ref_cnt == 0)
      delete this;
  }
};

template<class T>
class RefCntPtr {
private:
  T *ptr;

  RefCntPtr(const RefCntPtr<T> &other) {
    ptr = other.get();
    ptr->retain();
  }

  RefCntPtr<T> operator=(const RefCntPtr<T> &other) const {
    return RefCntPtr(other);
  }

  RefCntPtr(T *p=NULL) : ptr(p) {
    if (ptr)
      ptr->retain();
  }

  ~RefCntPtr(void) {
    if (ptr)
      ptr->release();
  }

  T *get(void) const {
    return ptr;
  }

  void reset(T *p=NULL) {
    if (ptr)
      ptr->release();
    ptr = p;
    if (ptr)
      ptr->retain();
  }

  T *operator->(void) const {
    return ptr;
  }
};

}

#endif  // __INCLUDE_UTIL_REFCNT_H__