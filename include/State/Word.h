#ifndef __INCLUDE_STATE_WORD_H__
#define __INCLUDE_STATE_WORD_H__

#include <Render/sp.h>

namespace tex {

union word {
  int64_t i64;
  void *ptr;
  sp scaled;
  glue_ratio g_ratio;
  uint64_t u64;
  unichar uc;

  bool operator==(word other) {
    return i64 == other.i64;
  }

  bool operator!=(word other) {
    return i64 != other.i64;
  }
};

}

#endif  // __INCLUDE_STATE_WORD_H__