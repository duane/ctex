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

#include <Render/VList.h>

using namespace tex;

typedef scaled_vector<6> delta;

RenderNode *tex::prune_page_top(UniquePtr<State> &state, RenderNode *vlist) {
  RenderNode *head = NULL;
  RenderNode *p = vlist;
  while (p) {
    switch (p->type) {
      case GLUE_NODE: {
        RenderNode *next = p->link;
        delete p;
        p = next;
        break;
      }
      // case RULE_NODE:
      case HBOX_NODE:
      case VBOX_NODE: {
        // we found the beginning of the page.
        head = p;
        glue_node split_top_skip_glue = skip_glue(state->mem(SPLIT_TOP_SKIP_CODE).scaled);
        RenderNode *split_top_skip = RenderNode::new_glue(split_top_skip_glue);
        split_top_skip->link = head;
        return split_top_skip;
      }
      default: {
        throw new GenericDiag("Unexpected node type in prune_page_top.",
                              DIAG_RENDER_ERR, BLAME_HERE);
      }
    }
  }
  // nothing but glue.
  return NULL;
}

RenderNode *tex::vert_break(UniquePtr<State> &state, RenderNode *vlist,
                            sp height, sp depth, sp &best_height_plus_depth) {
  RenderNode *p = vlist, *prev_p = vlist;

  RenderNode *best = NULL;
  int32_t best_badness = PENALTY_AWFUL;
  penalty_node penalty = 0;


  sp prev_depth = scaled(0), cur_height = scaled(0);

  delta active_height; // initialized to zero.


  // ready to break is set to true if a box or rule node is found, so that
  // when a glue node is encountered, it can break if ready_to_break is set.
  while (true) {
    bool found_break = false;
    bool update_heights = true;
    if (!p)
      penalty = PENALTY_BREAK;
    else {
      switch (p->type) {
        case GLUE_NODE: {
          update_heights = true;
          if (prev_p->type == HBOX_NODE
              //|| prev_p->type == RULE_NODE
              || prev_p->type == VBOX_NODE) {
            penalty = 0;
            found_break = true;
          }
          break;
        }
        // case RULE_NODE: 
        case HBOX_NODE:
        case VBOX_NODE: {
          cur_height += prev_depth + p->width(state);
          prev_depth = p->depth(state);
          break;
        }
        case PENALTY_NODE: {
          penalty = p->penalty;
          break;
        }
        default: {
          assert(false && "Found bad node type in vlist during vert_break.");
        }
      }
      if (found_break) {
        int32_t bad;
        if (penalty < PENALTY_INF) {
          if (cur_height < height) {
            if (active_height[2] || active_height[3] || active_height[4])
              bad = 0;
            else
              bad = badness(height - cur_height, active_height[1]);
          } else if (cur_height - height > active_height[5])
            bad = PENALTY_AWFUL;
          else
            bad = badness(cur_height - height, active_height[5]);
        }
        if (bad < PENALTY_AWFUL) {
          if (penalty <= PENALTY_BREAK)
            bad = penalty;
          else if (bad < PENALTY_INF)
            bad += penalty;
          else
            bad = PENALTY_DEPLORABLE;
        }
        if (bad <= best_badness) { // we've found a new breakpoint.
          best = p;
          best_badness = bad;
          best_height_plus_depth = cur_height + prev_depth;
        }
        if (bad == PENALTY_AWFUL || penalty <= PENALTY_BREAK)
          return best;
      }

      if (update_heights) {
        assert (p->type == GLUE_NODE && "Expected glue node!");
        glue_node &glue = p->glue;
        active_height[1+glue.stretch_order] += glue.stretch;
        active_height[5] +=  glue.shrink;

        if (glue.shrink_order != GLUE_NORMAL && glue.shrink != 0)
          throw new GenericDiag("Found infinite shrinkage!",
                                DIAG_RENDER_ERR, BLAME_HERE);
        cur_height = cur_height + prev_depth + glue.width;
        prev_depth = scaled(0);
      }

      prev_p = p;
      p = p->link;
    }
  }
  assert(false && "Unreachable code! Please fix.");
  return NULL;
}