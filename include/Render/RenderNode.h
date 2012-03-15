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

#ifndef __INCLUDE_RENDER_RENDERNODE_H__
#define __INCLUDE_RENDER_RENDERNODE_H__

#include <cassert>
#include <stdint.h>

#include <Diag/Diag.h>
#include <Render/Char.h>
#include <Render/Box.h>
#include <Render/Glue.h>
#include <Render/Kerning.h>
#include <Render/Penalty.h>
#include <Type/Font.h>
#include <Util/UniquePtr.h>

namespace tex {

class State;

enum {
  NULL_NODE = 0,
  HBOX_NODE,
  VBOX_NODE,
  CHAR_NODE,
  GLUE_NODE,
  KERN_NODE,
  LIG_NODE,
  PENALTY_NODE,
};

class RenderNode {
// allow copy/assign.
public:
  RenderNode *link;
  uint32_t type;
  union {
    char_node ch;
    glue_node glue;
    box_node box;
    penalty_node penalty;
    kern_node kern;
    lig_node lig;
  };

  sp width(UniquePtr<State> &state) const;
  sp height(UniquePtr<State> &state) const;
  sp depth(UniquePtr<State> &state) const;
  sp shift(UniquePtr<State> &state) const;

  static RenderNode *char_rnode(uint8_t uc, uint8_t f) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = CHAR_NODE;
    node->ch = (char_node){uc, f};
    return node;
  }

  static RenderNode *glue_rnode(sp width, sp stretch, sp shrink,
                    glue_order stretch_order, glue_order shrink_order) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = GLUE_NODE;
    node->glue = (glue_node){width, stretch, shrink,
                             stretch_order, shrink_order};
    return node;
  }

  static RenderNode *new_glue(glue_node &g) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = GLUE_NODE;
    node->glue = g;
    return node;
  }

  static RenderNode *new_kern(kern_type type, sp width) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = KERN_NODE;
    node->kern = (kern_node){type, width};
    return node;
  }

  static RenderNode *new_lig(uint32_t code, uint32_t font, RenderNode *inner)
  {
    RenderNode *node = new RenderNode;
    node->type = LIG_NODE;
    node->lig = (lig_node){code, font, inner};
    return node;
  }

  static RenderNode *empty_hbox(void) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = HBOX_NODE;
    node->box = empty_box();
    return node;
  }

  static RenderNode *hbox(box_node &box) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = HBOX_NODE;
    node->box = box;
    return node;
  }

  static RenderNode *empty_vbox(void) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = HBOX_NODE;
    node->box = empty_box();
    return node;
  }

  static RenderNode *vbox(box_node &box) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = VBOX_NODE;
    node->box = box;
    return node;
  }

  void print(UniquePtr<State> &state, unsigned indent);
};

}  // namespace tex

#endif  // __INCLUDE_RENDER_RENDERNODE_H__