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

#include <Render/RenderNode.h>
#include <State/State.h>

using namespace tex;

 sp RenderNode::width(UniquePtr<State> &state) const {
  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE:
      return box.width;
    case GLUE_NODE:
      return glue.width;
    case CHAR_NODE:
      return state->metrics(ch.font).get(ch.ch).width;
    default:
      assert(false && "Attempted to fetch width of widthless node.");
  }
  assert(false && "Unreachable! Fix code.");
  return scaled(0);
}

sp RenderNode::height(UniquePtr<State> &state) const {
  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE:
      return box.height;
    case GLUE_NODE:
      return glue.width; // cheating.
    case CHAR_NODE:
      return state->metrics(ch.font).get(ch.ch).height;
    default:
      assert(false && "Attempted to fetch width of heightless node.");
  }
  assert(false && "Unreachable! Fix code.");
  return scaled(0);
}

sp RenderNode::depth(UniquePtr<State> &state) const {
  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE:
      return box.depth;
    case CHAR_NODE:
      return state->metrics(ch.font).get(ch.ch).depth;
    default:
      assert(false && "Attempted to fetch width of depthless node.");
  }
  assert(false && "Unreachable! Fix code.");
  return scaled(0);
}

sp RenderNode::shift(UniquePtr<State> &state) const {
  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE:
      return box.width;
    default:
      assert(false && "Attempted to fetch width of shiftless node.");
  }
  assert(false && "Unreachable! Fix code.");
  return scaled(0);
}
