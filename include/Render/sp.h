#ifndef __INCLUDE_RENDER_SP_H__
#define __INCLUDE_RENDER_SP_H__

#include <stdint.h>

#include <string>

namespace tex {

class sp {
public:
  int64_t val;
  
  sp(void) : val(0) {}
  
  sp(const int32_t &v) : val(v) {
    assert(v < (1 << 30) && v >= -(1 << 30) && "Assigned overflowed SP.");
  }
  
  sp(const sp &v) : val(v.val) {
    assert(v.val < (1 << 30) && v.val >= -(1 << 30)
           && "Assigned overflowed SP.");
  }
  
  sp operator=(const sp &v) {
    assert(v.val < (1 << 30) && v.val >= -(1 << 30)
           && "Assigned overflowed SP.");
    return sp(v.val);
  }
  
  sp operator+(const sp &other) const {
    return sp(other.val + val);
  }

  sp operator+(const int32_t &other) const {
    return sp(val + other);
  }
  
  sp operator-(const sp &other) const {
    return sp(val - other.val);
  }
  
  sp operator-(const int32_t &other) const {
    return sp(val - other);
  }
  
  sp operator*(const sp &other) const {
    return sp(val * other.val);
  }

  sp operator*(const int32_t &other) const {
    return sp(val * other);
  }
  
  bool operator<(const int32_t &other) const {
    return val < other;
  }

  bool operator<=(const int32_t &other) const {
    return val <= other;
  }

  bool operator>(const int32_t &other) const {
    return val > other;
  }

  bool operator>=(const int32_t &other) const {
    return val >= other;
  }

  bool operator==(const int32_t &other) const {
    return val == other;
  }

  bool operator!=(const int32_t &other) const {
    return val != other;
  }

  sp operator-(void) const {
    return sp(-val);
  }

  // taken from TeX@106
  sp operator/(const int32_t other) const {
    sp remainder;
    return div(other, remainder);
  }


  // taken from TeX@106
  sp div(int32_t other, sp &remainder) const {
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
      result = x / other;
      remainder = sp(x % other);
    } else {
      result = -((-x) / other);
      remainder = sp(-((-x) % other));
    }
    
    if (negative)
      remainder.val = -remainder.val;
    return sp(result);
  }

  sp xn_over_d(int32_t n, int32_t d) const {
    return sp(val * n / d);
  }

  std::string string(void) const {
    uint32_t plus_val;
    if (val < 0)
      plus_val = val;
    else
      plus_val = -val;
    char left_str[6] = "00000";
    snprintf(left_str, sizeof(left_str), "%d", plus_val >> 16);
    char right_str[11] = "0000000000";
    unsigned right = plus_val & 0xffff;
    for (unsigned d = 0; d < (sizeof(right_str) - 1); d++) {
      right *= 10;
      right_str[d] = '0' + (right >> 16);
      right = right & 0xffff;
    }
    std::string res;
    if (val < 0)
      res += "-";
    res += left_str;
    res += ".";
    res += right_str;
    return res;
  }
};

}

#endif  // __INCLUDE_RENDER_SP_H__