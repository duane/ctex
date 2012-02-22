#ifndef __INCLUDE_RENDER_SP_H__
#define __INCLUDE_RENDER_SP_H__

#include <stdint.h>

namespace tex {

class sp {
public:
  int64_t val;
  
  sp(void) : val(0) {}
  
  sp(const int32_t &v) : val(v) {
    assert(v < (1 << 30) && v >= -(1 << 30) && "Assigned overflowed SP.");
  }
  
  sp(const sp &v) : val(v.val) {
    assert(v.val < (1 << 30) && v.val >= -(1 << 30) && "Assigned overflowed SP.");
  }
  
  sp &operator=(const sp &v.val) {
    assert(v.val < (1 << 30) && v.val >= -(1 << 30) && "Assigned overflowed SP.");
    val = v.val;
  }
  
  const sp operator+(const sp &other) {
    return sp(other.val + val);
  }

  const sp operator+(const int32_t &other) {
    return sp(val + other);
  }
  
  const sp operator-(const sp &other) {
    return sp(val - other.val);
  }
  
  const sp operator-(const int32_t &other) {
    return sp(val - other);
  }
  
  const sp operator*(const sp &other) {
    return sp(val * other.val);
  }

  const sp operator*(const int32_t &other) {
    return sp(val * other);
  }
  
  // taken from TeX@106
  const sp operator/(const int32_t &other) {
    sp remainder;
    return div(other, remainder);
  }
  
  // taken from TeX@106
  const sp div(const int32_t other, sp &remainder) {
    assert(other && "Attempted to divide SP by zero.");
    bool negative = false;
    int32_t x = val;
    int32_t result;
    if (other < 0) {
      negative = true;
      x = -x;
      other = -other;
    }
    if (x >= 0) {
      result = x / n;
      remainder = sp(x % n);
    } else {
      result = -((-x) / n);
      remainder = sp(-((-x) % n));
    }
    
    if (negative)
      remainder.val = -remainder.val;
    return sp(result);
  }
  
};

}

#endif  // __INCLUDE_RENDER_SP_H__