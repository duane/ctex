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

#include <iostream>

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
    case LIG_NODE:
      return state->metrics(lig.font).get(lig.code).width;
    case KERN_NODE:
      return kern.width;
    case RULE_NODE:
      return rule.width;
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
    case LIG_NODE:
      return state->metrics(lig.font).get(lig.code).height;
    case RULE_NODE:
      return rule.height;
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
    case LIG_NODE:
      return state->metrics(lig.font).get(lig.code).depth;
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

void RenderNode::print(UniquePtr<State> &state, unsigned indent) {
  std::string indentation;
  for (unsigned i = 0; i < indent; i++) {
    indentation += "\t";
  }

  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE: {
      std::cout << indentation;
      if (type == HBOX_NODE)
        std::cout << "Horizontal box:" << "\n";
      else
        std::cout << "Vertical box:" << "\n";
      std::cout << indentation << "Width: " << box.width.string() << "\n";
      std::cout << indentation << "Height: " << box.height.string() << "\n";
      std::cout << indentation << "Depth: " << box.depth.string() << "\n";
      std::cout << indentation << "Shift: " << box.shift.string() << "\n";
      RenderNode *node = box.list;
      while (node) {
        node->print(state, indent + 1);
      }
      break;
    }
    case GLUE_NODE: {
      std::cout << indentation << "Glue: " << glue.width.string();
      if (glue.stretch) {
        switch(glue.stretch_order) {
          case GLUE_NORMAL:
            std::cout << " plus ";
            break;
          case GLUE_FIL:
            std::cout << " fil ";
            break;
          case GLUE_FILL:
            std::cout << " fill ";
            break;
          case GLUE_FILLL:
            std::cout << " filll ";
            break;
        }
      std::cout << glue.stretch.string();
      }
      if (glue.shrink) {
        switch(glue.shrink_order) {
          case GLUE_NORMAL:
            std::cout << " minus ";
            break;
          case GLUE_FIL:
            std::cout << " hs ";
            break;
          case GLUE_FILL:
            std::cout << " hss ";
            break;
          case GLUE_FILLL:
            std::cout << " hsss ";
            break;
        }
        std::cout << glue.shrink.string();
      }
      std::cout << "\n";
      break;
    }
    case CHAR_NODE: {
      std::cout << indentation << "Char 0x";
      std::cout.flush();
      printf("%0X\n", ch.ch);
      fflush(stdout);
      break;
    }
    case LIG_NODE: {
      std::cout << indentation << "Lig 0x";
      std::cout.flush();
      printf("%0x\n", lig.code);
      fflush(stdout);
      break;
    }
    case PENALTY_NODE: {
      std::cout << indentation << "Penalty " << penalty << "\n";
      break;
    }
    case KERN_NODE: {
      std::cout << indentation << "Kern " << kern.width.string() << "\n";
    }
  }
}
