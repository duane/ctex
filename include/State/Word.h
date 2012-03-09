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