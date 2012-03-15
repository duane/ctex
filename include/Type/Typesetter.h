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
  OP_KERN,
  OP_SET,
  OP_SET_LIG,
};

struct set_ligature {
  uint32_t code;
  uint32_t idx, extent;
};

struct set_op {
  set_op_type type;
  union {
    sp kern;
    uint32_t code;
    set_ligature lig;
  };

  static set_op set(uint32_t code) {
    set_op op;
    op.type = OP_SET;
    op.code = code;
    return op;
  }

  static set_op set_lig(uint32_t code, uint32_t idx, uint32_t extent) {
    set_op op;
    set_ligature lig;
    lig.code = code;
    lig.idx = idx;
    lig.extent = extent;
    op.type = OP_SET_LIG;
    op.lig = lig;
    return op;
  }

  static set_op kerning(sp kern) {
    set_op op;
    op.type = OP_KERN;
    op.kern = kern;
    return op;
  }
};

}

#endif  // __INCLUDE_TYPE_TYPESETTER_H__