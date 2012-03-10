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

#ifndef __INCLUDE_RENDER_BOX_H__
#define __INCLUDE_RENDER_BOX_H__

#include <Render/Glue.h>
#include <Util/UniquePtr.h>

namespace tex {

class RenderNode;
class State;

struct box_node {
  sp width, depth, height, shift;
  RenderNode *list;
  glue_order order;
  glue_sign sign;
  glue_ratio g_ratio;
};

static inline box_node empty_box(void) {
  return (box_node){scaled(0), scaled(0), scaled(0), scaled(0),
                    NULL, GLUE_NORMAL, SIGN_NORMAL, 0.0};
}

enum pack_type {
  EXACTLY,
  ADDITIONAL,
};

extern RenderNode *vpackage(UniquePtr<State> &state, RenderNode *vlist,
                            sp height, sp depth, pack_type type);

static inline RenderNode *vpack(UniquePtr<State> &state, RenderNode *vlist,
                            sp height, pack_type type) {
  return vpackage(state, vlist, height, scaled((1<<30) - 1), type);
}

extern RenderNode *hpack(UniquePtr<State> &state, RenderNode *hlist, sp width,
                         pack_type type);

}  // namespace tex

#endif  //  __INCLUDE_RENDER_BOX_H__