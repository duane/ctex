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

#ifndef __INCLUDE_RENDER_SP_H__
#define __INCLUDE_RENDER_SP_H__

#include <cassert>
#include <stdint.h>

#include <string>

namespace tex {

class sp {
public:
  int64_t i64;
  
  sp operator=(int v) {
    sp scaled;
    scaled.i64 = v;
    return scaled;
  }

  sp operator+(const sp other) const {
    return (sp){other.i64 + i64};
  }

  sp operator+(const int32_t other) const {
    return (sp){i64 + other};
  }
  
  sp operator-(const sp other) const {
    return (sp){i64 - other.i64};
  }
  
  sp operator-(const int32_t other) const {
    return (sp){i64 - other};
  }
  
  sp operator*(const sp other) const {
    return (sp){(i64 * other.i64) >> 16};
  }

  sp operator*(const int32_t other) const {
    return (sp){i64 * other};
  }
  
  void operator+=(const int32_t other) {
    i64 += other;
  }

  void operator+=(const sp other) {
    i64 += other.i64;
  }

  void operator-=(const int32_t other) {
    i64 -= other;
  }

  void operator-=(const sp other) {
    i64 -= other.i64;
  }

  bool operator<(const int32_t other) const {
    return i64 < other;
  }

  bool operator<(const sp other) const {
    return i64 < other.i64;
  }

  bool operator<=(const int32_t other) const {
    return i64 <= other;
  }

  bool operator<=(const sp other) const {
    return i64 <= other.i64;
  }

  bool operator>(const int32_t other) const {
    return i64 > other;
  }

  bool operator>(const sp other) const {
    return i64 > other.i64;
  }

  bool operator>=(const int32_t other) const {
    return i64 >= other;
  }

  bool operator>=(const sp other) const {
    return i64 >= other.i64;
  }

  bool operator==(const int32_t other) const {
    return i64 == other;
  }

  bool operator==(const sp other) const {
    return i64 == other.i64;
  }

  bool operator!=(const int32_t other) const {
    return i64 != other;
  }

  bool operator!=(const sp other) const {
    return i64 != other.i64;
  }

  sp operator-(void) const {
    return (sp){-i64};
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
    int32_t x = i64;
    int32_t result;
    if (other < 0) {
      negative = true;
      x = -x;
      other = -other;
    }
    if (x >= 0) {
      result = x / other;
      remainder = (sp){x % other};
    } else {
      result = -((-x) / other);
      remainder = (sp){-((-x) % other)};
    }
    
    if (negative)
      remainder.i64 = -remainder.i64;
    return (sp){result};
  }

  sp xn_over_d(int32_t n, int32_t d) const {
    return (sp){i64 * n / d};
  }

  std::string string(void) const {
    uint32_t plus_val;
    if (i64 < 0)
      plus_val = -i64;
    else
      plus_val = i64;
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
    if (i64 < 0)
      res += "-";
    res += left_str;
    res += ".";
    res += right_str;
    return res;
  }

  operator bool(void) const {
    return i64 != 0;
  }
};

static inline sp scaled(int32_t v) {
  sp result = (sp){v};
  return result;
}

static inline sp scaledf(float f) {
  return scaled((int32_t)(f * 0x10000));
}

template <unsigned N>
class scaled_vector {
private:
  typedef scaled_vector<N> vec_type;

  sp vec[N];
public:

  void set_all(sp rhs) {
    for (unsigned i = 0; i < N; i++) {
      vec[i] = rhs;
    }
  }
  
  void operator+=(const vec_type &other) {
    for (unsigned i = 0; i < N; i++) {
      vec[i] = vec[i] + other.vec[i];
    }
  }

  void operator-=(const vec_type &other) {
    for (unsigned i = 0; i < N; i++) {
      vec[i] = vec[i] - other.vec[i];
    }
  }

  vec_type operator+(const vec_type &other) {
    vec_type result;
    for (unsigned i = 0; i < N; i++){
      result.vec[i] = vec[i] + other.vec[i];
    }
    return result;
  }

  vec_type operator-(const vec_type &other) {
    vec_type result;
    for (unsigned i = 0; i < N; i++) {
      result.vec[i] = vec[i] - other.vec[i];
    }
    return result;
  }

  sp &operator[](unsigned idx) {
    assert(idx < N && "Attempted to access scaled_vector out of bounds.");
    return vec[idx];
  }
};

}

#endif  // __INCLUDE_RENDER_SP_H__