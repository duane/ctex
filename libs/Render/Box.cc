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

#include <Render/Box.h>
#include <Render/RenderNode.h>
#include <State/State.h>

#include <iostream>

using namespace tex;

RenderNode *tex::hpack(UniquePtr<State> &state,
                       RenderNode *hlist, sp width, pack_type type) {
  RenderNode *result_node = RenderNode::empty_hbox();
  box_node &hbox = result_node->box;
  hbox.list = hlist;
  RenderNode *p = hlist, *prev_p = NULL;
  sp total_stretch[4] = {scaled(0), scaled(0), scaled(0), scaled(0)};
  sp total_shrink[4] = {scaled(0), scaled(0), scaled(0), scaled(0)};
  while (p) {
    if (p->type == CHAR_NODE || p->type == LIG_NODE) {
      while (p && (p->type == CHAR_NODE || p->type == LIG_NODE)) {
        sp width = p->width(state);
        sp height = p->height(state);
        sp depth = p->depth(state);
        hbox.width += width;
        if (height > hbox.height)
          hbox.height = height;
        if (depth > hbox.depth)
          hbox.depth = depth;
        p = p->link;
      }
    }
    if (!p)
      break;
    switch (p->type) {
      case RULE_NODE: {
        sp height = p->height(state);
        if (height > hbox.height)
          hbox.height = height;
        hbox.width += p->width(state);
        break;
      }
      case HBOX_NODE:
      case VBOX_NODE: {
        hbox.width += p->width(state);
        sp s = p->shift(state);
        sp h = p->height(state);
        sp d = p->depth(state);
        if ((h - s) > hbox.height)
          hbox.height = h - s;
        if ((d + s) > hbox.depth)
          hbox.depth = h + s;
        break;
      }
      case GLUE_NODE: {
        hbox.width += p->width(state);
        glue_node &glue = p->glue;
        total_stretch[glue.stretch_order] += glue.stretch;
        total_shrink[glue.shrink_order] += glue.shrink;
        break;
      }
      case KERN_NODE: {
        hbox.width += p->width(state);
        break;
      }
      case PENALTY_NODE: {
        break;
      }
      default:
        assert(false && "Attempted to pack unknown RenderNode type.");
    }
    prev_p = p;
    p = prev_p->link;
  }

  // hbox.width is now the "natural" width of the box
  sp target_width; // the width the hbox will be in the end
  if (type == EXACTLY)
    target_width = width;
  else
    target_width = hbox.width + width;
  sp shortfall = target_width - hbox.width;
  if (shortfall == 0) {
    hbox.order = GLUE_NORMAL;
    hbox.sign = SIGN_NORMAL;
    hbox.g_ratio = 0.0;
    return result_node;
  }
  if (shortfall < 0) { // shrink
    glue_order ord;
    if (total_shrink[GLUE_FILLL] != 0)
      ord = GLUE_FILLL;
    else if (total_shrink[GLUE_FILL] != 0)
      ord = GLUE_FILL;
    else if (total_shrink[GLUE_FIL] != 0)
      ord = GLUE_FIL;
    else
      ord = GLUE_NORMAL;

    if (total_shrink[ord] == 0) {
      hbox.order = GLUE_NORMAL;
      hbox.sign = SIGN_NORMAL;
      hbox.g_ratio = 0.0;
      return result_node;
    } else {
      hbox.order = ord;
      hbox.sign = SIGN_SHRINK;
      hbox.g_ratio = (float)(-shortfall.i64)/total_shrink[ord].i64;
      if (hbox.order == GLUE_NORMAL
          && (-shortfall.i64) > total_shrink[GLUE_NORMAL].i64) {
        hbox.g_ratio = 1.0;
        RenderNode *overfull_rule
          = RenderNode::new_rule(scaled(5<<16), hbox.height);
        prev_p->link = overfull_rule;
        hbox.width += overfull_rule->rule.width;
        std::cout << "Overfull box, exceeds line by"
                  << scaled(-shortfall.i64 - total_shrink[GLUE_NORMAL].i64)
                      .string()
                  << "pt.\n";
      }
      return result_node;
    }
  } else { // stretch
    glue_order ord;
    if (total_stretch[GLUE_FILLL] != 0)
      ord = GLUE_FILLL;
    else if (total_stretch[GLUE_FILL] != 0)
      ord = GLUE_FILL;
    else if (total_stretch[GLUE_FIL] != 0)
      ord = GLUE_FIL;
    else
      ord = GLUE_NORMAL;

    if (total_stretch[ord] == 0) {
      hbox.order = GLUE_NORMAL;
      hbox.sign = SIGN_NORMAL;
      hbox.g_ratio = 0.0;
      return result_node;
    } else {
      hbox.order = ord;
      hbox.sign = SIGN_STRETCH;
      hbox.g_ratio = (float)shortfall.i64/total_stretch[ord].i64;
      if (hbox.order == GLUE_NORMAL
          && shortfall > total_stretch[GLUE_NORMAL]) {
        hbox.g_ratio = 1.0;
        RenderNode *underfull_rule
          = RenderNode::new_rule(scaled(5<<16), hbox.height);
        prev_p->link = underfull_rule;
        hbox.width += underfull_rule->rule.width;
        std::cout << "Underfull box, "
                  << (shortfall - total_stretch[GLUE_NORMAL]).string()
                  << "pt short of line.\n";
      }
      return result_node;
    }
  }
  assert(false && "Unreachable! please fix.");
  return NULL;
}

RenderNode *tex::vpackage(UniquePtr<State> &state, RenderNode *vlist,
                          sp height, sp given_depth, pack_type type) {
  sp nat_height = scaled(0);
  UniquePtr<RenderNode> node;
  node.reset(RenderNode::empty_vbox());

  box_node &box = node->box;
  box.list = vlist;
  RenderNode *p = box.list;
  sp total_stretch[4] = {scaled(0), scaled(0), scaled(0), scaled(0)};
  sp total_shrink[4] = {scaled(0), scaled(0), scaled(0), scaled(0)};

  while (p) {
    switch(p->type) {
      case CHAR_NODE:
      case LIG_NODE: {
        assert(false && "Found char/lig node in vlist.");
      }
      case RULE_NODE: {
        nat_height += p->height(state);
        sp width;
        if (width > box.width)
          box.width = width;
        break;
      }
      case VBOX_NODE:
      case HBOX_NODE: {
        box_node inner_box = p->box;
        nat_height += box.depth + inner_box.height;
        box.depth = inner_box.depth;
        if (inner_box.shift + inner_box.width > box.width)
          box.width = inner_box.shift + inner_box.width;
        break;
      }
      case GLUE_NODE: {
        glue_node &glue = p->glue;
        nat_height += glue.width;
        total_stretch[glue.stretch_order] += glue.stretch;
        total_shrink[glue.shrink_order] += glue.shrink;
        break;
      }
      case PENALTY_NODE: {
        break;
      }
      default: {
        assert(false && "Found bad node type in vlist.");
      }
    }
    p = p->link;
  }

  if (box.depth > given_depth) {
    nat_height += (box.depth - given_depth);
    box.depth = given_depth;
  }

  // now sort out target height.
  if (type == ADDITIONAL)
    height += nat_height;
  box.height = height;
  sp excess = height - nat_height;
  if (excess == 0) {
    box.sign = SIGN_NORMAL;
    box.order = GLUE_NORMAL;
    box.g_ratio = 0.0;
    return node.take();
  }
  if (excess > 0) {
    if (total_stretch[GLUE_FILLL] != 0)
      box.order = GLUE_FILLL;
    else if (total_stretch[GLUE_FILL] != 0)
      box.order = GLUE_FILL;
    else if (total_stretch[GLUE_FIL] != 0)
      box.order = GLUE_FIL;
    else
      box.order = GLUE_NORMAL;
    box.sign = SIGN_STRETCH;
    if (total_stretch[box.order] != 0)
      box.g_ratio = (float)excess.i64/total_stretch[box.order].i64;
    else {
      box.sign = SIGN_NORMAL;
      box.g_ratio = 0.0;
    }
  } else {
    if (total_shrink[GLUE_FILLL] != 0)
      box.order = GLUE_FILLL;
    else if (total_shrink[GLUE_FILL] != 0)
      box.order = GLUE_FILL;
    else if (total_shrink[GLUE_FIL] != 0)
      box.order = GLUE_FIL;
    else
      box.order = GLUE_NORMAL;
    box.sign = SIGN_SHRINK;
    if (total_shrink[box.order] != 0)
      box.g_ratio = (float)excess.i64/total_stretch[box.order].i64;
    else {
      box.sign = SIGN_NORMAL;
      box.g_ratio = 0.0;
    }
  }
  return node.take();
}