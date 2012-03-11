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

#ifndef __INCLUDE_RENDER_VLIST_H__
#define __INCLUDE_RENDER_VLIST_H__

#include <State/State.h>

namespace tex {


extern RenderNode *prune_page_top(UniquePtr<State> &state, RenderNode *vlist);
extern RenderNode *vert_break(UniquePtr<State> &state, RenderNode *vlist,
                              sp height, sp depth, sp &best_height_plus_depth);
extern RenderNode *vsplit(UniquePtr<State> &state, uint8_t box_reg, sp height);

}

#endif  // __INCLUDE_RENDER_VLIST_H__