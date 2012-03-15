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

#ifndef __INCLUDE_TYPE_TYPESETTER_H__
#define __INCLUDE_TYPE_TYPESETTER_H__

#include <Render/sp.h>

namespace tex {

enum set_op_type {
  OP_ADJUST,
  OP_SET,
};

struct set_adjust {
  sp h, v;
};

struct set_op {
  set_op_type type;
  union {
    set_adjust adjustment;
    uint32_t code;
  };

  static set_op set(uint32_t code) {
    set_op op;
    op.type = OP_SET;
    op.code = code;
    return op;
  }

  static set_op adjust(sp h, sp v) {
    set_op op;
    op.type = OP_ADJUST;
    op.adjustment.h = h;
    op.adjustment.v = v;
    return op;
  }
};

}

#endif  // __INCLUDE_TYPE_TYPESETTER_H__