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

#include <Render/SimpleBreaker.h>

#include <iostream>

using namespace tex;

static inline void finalize_line(UniquePtr<State> &state, RenderNode *head,
                                 RenderNode *tail) {
  // first, throw left and right skip in there.
  assert(head && "Attempted to package a NULL line.");
  tex_eqtb &eqtb = state->eqtb();
  glue_node left_glue = skip_glue(eqtb[LEFT_SKIP_CODE].scaled);
  RenderNode *left = RenderNode::new_glue(left_glue);
  glue_node right_glue = skip_glue(eqtb[RIGHT_SKIP_CODE].scaled);
  RenderNode *right = RenderNode::new_glue(right_glue);
  left->link = head;
  tail->link = right;
  RenderNode *hbox = hpack(state,left, eqtb[HSIZE_CODE].scaled,
                           EXACTLY);
  // now append it to the vertical list.
  state->render().append(hbox);
  sp height = hbox->height(state);
  sp baseline = eqtb[BASELINE_SKIP_CODE].scaled;
  if (baseline > height) {
    glue_node baseline_glue = skip_glue(baseline - height);
    RenderNode *baseline = RenderNode::new_glue(baseline_glue);
    state->render().append(baseline);
  }
}

extern void tex::simple_line_break(UniquePtr<State> &state) {
  // first, grab the hlist.
  RenderNode *head = state->render().head();
  if (!head)
    return;
  RenderNode *tail = state->render().tail();
  // now leave horizontal mode, so we can append lines as hboxes to the vertical list.
  state->render().pop();

  tex_eqtb &eqtb = state->eqtb();

  RenderNode *cur_node, *break_node, *newline_node, *prev_node;
  cur_node = head;
  prev_node = NULL;
  break_node = NULL;
  newline_node = NULL;

  sp left_skip = eqtb[LEFT_SKIP_CODE].scaled;
  sp right_skip = eqtb[RIGHT_SKIP_CODE].scaled;
  sp max_width = eqtb[HSIZE_CODE].scaled - left_skip - right_skip;
  sp cur_width = scaled(0), break_width = scaled(0), newline_width = scaled(0);
  uint32_t line = 0;
  bool skip_spaces = true;
  while (cur_node) {
    switch(cur_node->type) {
      case GLUE_NODE: {
        if (skip_spaces) {
          break_width = break_width + cur_node->width(state);
        } else {
          // we've found a new breakpoint.
          // update the current line from the last breakpoint up until, but not including, cur_node.
          break_node = prev_node;
          newline_node = NULL;
          cur_width = cur_width + break_width + newline_width;
          break_width = cur_node->width(state);
          newline_width = 0;
          skip_spaces = true;
        }
        break;
      }
      case VBOX_NODE:
      case RULE_NODE:
      case CHAR_NODE:
      case KERN_NODE:
      case LIG_NODE: {
        if (skip_spaces) {
          newline_node = cur_node;
          newline_width = cur_node->width(state);
          skip_spaces = false;
        } else {
          newline_width += cur_node->width(state);
        }
        break;
      }
      default : {
        throw new GenericDiag("Found bad type of render node in simple line "
                              "breaker.", DIAG_RENDER_ERR, BLAME_HERE);
      }
    }

    sp total_line_width = cur_width + break_width + newline_width;
    if (break_node && newline_node && total_line_width >= max_width) {
      RenderNode *break_after = break_node->link;
      finalize_line(state, head, break_node);
      head = break_after;
      // delete glue until we get to newline.
      while (head != newline_node) {
        RenderNode *next = head->link;
        delete head;
        head = next;
      }
      newline_node = break_node = NULL;
      break_width = scaled(0);
      cur_width = newline_width;
      newline_width = scaled(0);
      line++;
    }

    prev_node = cur_node;
    cur_node = prev_node->link;
  }

  if (head) {
    // finalize the last line.
    if (newline_node)
      finalize_line(state, head, tail);
    else {
      if (break_node) {
        RenderNode *glue = break_node->link;
        finalize_line(state, head, break_node);
        while (glue) {
          RenderNode *node = glue->link;
          delete glue;
          glue = node;
        }
      } else {
        finalize_line(state, head, tail);
      }
    }
  }
}