#ifndef __INCLUDE_STATE_WORD_H__
#define __INCLUDE_STATE_WORD_H__

#include <Render/sp.h>

namespace tex {

union word {
  void *ptr;
  sp scaled;
  glue_ratio g_ratio;
  int64_t i64;
  uint64_t u64;
};

}

#endif  // __INCLUDE_STATE_WORD_H__