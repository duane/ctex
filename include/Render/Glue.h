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

#ifndef __INCLUDE_RENDER_GLUE_H__
#define __INCLUDE_RENDER_GLUE_H__

#include <stdint.h>

#include <Render/sp.h>

namespace tex {

typedef float glue_ratio;

enum glue_order {
  GLUE_NORMAL = 0,
  GLUE_FIL,
  GLUE_FILL,
  GLUE_FILLL
};

enum glue_sign {
  SIGN_NORMAL = 0,
  SIGN_STRETCH,
  SIGN_SHRINK,
};

struct glue_node {
  sp width, stretch, shrink;
  glue_order stretch_order, shrink_order;
};

static inline glue_node skip_glue(sp width) {
  return (glue_node){width, scaled(0), scaled(0), GLUE_NORMAL, GLUE_NORMAL};
}

}

#endif  // __INCLUDE_RENDER_GLUE_H__