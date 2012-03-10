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

#ifndef __INCLUDE_RENDER_PENALTY_H__
#define __INCLUDE_RENDER_PENALTY_H__

namespace tex {

typedef int32_t penalty_node;

enum {
  PENALTY_BREAK = -10000,
  PENALTY_INF = 10000,
  PENALTY_DEPLORABLE = 100000,
  PENALTY_AWFUL = 1000000,
};

static inline int32_t badness(sp t, sp s) {
  int32_t r;
  if (t == 0)
    return 0;
  else if (s <= 0)
    return PENALTY_INF;
  else {
    if (t <= 7230584)
      r = (t.i64 * 297) / s.i64;
    else if (s >= 1663497)
      r = t.i64 / (s.i64 / 297);
    else
      r = t.i64;
    if (r > 1290)
      return PENALTY_INF;
    return (r * r * r + 0x20000) / 0x40000;
  }

}

}

#endif  // __INCLUDE_RENDER_PENALTY_H__