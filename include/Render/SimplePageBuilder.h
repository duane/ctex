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

#ifndef __INCLUDE_RENDER_SIMPLEPAGEBUILDER_H__
#define __INCLUDE_RENDER_SIMPLEPAGEBUILDER_H__

#include <Render/sp.h>
#include <Util/UniquePtr.h>

namespace tex {

class RenderNode;
class State;

class SimplePageBuilder {
private:
  SimplePageBuilder(const SimplePageBuilder&);
  SimplePageBuilder &operator=(const SimplePageBuilder&);

private:
  void ship_page(UniquePtr<State> &state);

public:
  RenderNode *p_head, *p_tail;

  sp cur_height, break_height;
  RenderNode *break_head, *break_tail;

  SimplePageBuilder(void) : p_head(NULL), p_tail(NULL), cur_height(scaled(0)),
                          break_height(scaled(0)), break_head(NULL),
                          break_tail(NULL) {}

  void build_page(UniquePtr<State> &state);
};

}

#endif  // __INCLUDE_RENDER_SIMPLEPAGEBUILDER_H__