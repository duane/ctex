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

static inline void update_delta_glue(delta &d, RenderNode *node) {
  assert(node.type == GLUE_NODE
         && "Expected glue node, got something different.");
  glue_node &glue = node.glue;
  d[1+glue.stretch_order] += glue.stretch;
  d[5] +=  glue.shrink;
}

RenderNode *tex::vert_break(UniquePtr<State> &state, RenderNode *vlist,
                            sp height, sp depth) {
  RenderNode *p = vlist, *prev_p = vlist;

  RenderNode *best = NULL;
  int32_t best_badness = BADNESS_AWFUL;
  int32_t penalty = 0;


  sp prev_depth = scaled(0), curr_height = scaled(0);

  delta active_height; // initialized to zero.

  bool done = false;
  bool ready_to_break = false;
  while (!done) {
    if (!p)
      penalty = BADNESS_EJECT;
    else {
      switch (p->type) {
        case GLUE_NODE: {
          if (ready_to_break)
            penalty = 0;
          else {
            update_delta_glue(active_height, p);
          }
          break;
        }
        // case RULE_NODE: 
        case HBOX_NODE:
        case VBOX_NODE: {

        }
      }
    }
  }
}