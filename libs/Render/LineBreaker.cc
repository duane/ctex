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

#include <Render/LineBreaker.h>
#include <Render/RenderNode.h>
#include <Render/SimpleBreaker.h>



#include <list>


namespace tex {

typedef scaled_vector<6> delta;

enum active_type {
  ACTIVE,
  DELTA,
};

enum fitness_type {
  VERY_LOOSE_FIT,
  LOOSE_FIT,
  DECENT_FIT,
  TIGHT_FIT,
  MAX_FITNESS,
};

struct passive_node {
  RenderNode *break_point;
  passive_node *parent;
  std::list<passive_node> children;
};

struct active_node {
  active_type type;
  active_node *link;
  union {
    struct {
      passive_node *passive;
      fitness_type fitness;
      int32_t demerits;
    };
    sp delta[6];
  };
};

enum {
  DELTA_WIDTH = 0,
  DELTA_NORMAL,
  DELTA_FIL,
  DELTA_FILL,
  DELTA_FILLL,
  DELTA_SHRINK,
};

class LineBreaker {
private:
  delta active_width, cur_active_width, background, break_width;
  RenderNode *head, *tail;

  active_node *

  int32_t minimal_demerits[MAX_FITNESS];
  int32_t minimum_demerits;
  int32_t threshold;

public:
  void break_line(UniquePtr<State> &state) {
    RenderState &render = state->render();
    assert(render.mode() == HMODE && "Attempted to break paragraph in wrong mode.");
    head = render.head();
    tail = render.tail();
    render.pop();
    render.set_mode(VMODE);
    if (!head)
      return;
    assert(tail && "Found NULL tail; tail should never be NULL when head is non-null.");

    /*if (tail->type == GLUE_NODE) {
      tail->type = PENALTY_NODE;
      tail->penalty = PENALTY_INF;
    } else {
      tail->link = RenderNode::new_penalty(PENALTY_INF);
      tail = tail->link;
      tail->link = NULL;
    }*/

    RenderNode *p = head;
    RenderNode *prev_p = NULL;
    while (p) {
      switch (p->type) {
        case GLUE_NODE: {
          if (p->glue.shrink_order != GLUE_NORMAL)
            throw new GenericDiag("Encountered infinite shrinkage in HMODE.",
                                  DIAG_RENDER_ERR, BLAME_HERE);
          if (prev_p && prev_p->type != GLUE_NODE) {
            RenderNode *break_rule
              = RenderNode::new_rule(scaled(1<<16), scaled(10<<16));
            break_rule->link = p;
            prev_p->link = break_rule;
          }
          active_width[0] += p->glue.width;
          active_width[1+p->glue.stretch_order] += p->glue.stretch;
          active_width[5] += p->glue.shrink;
          break;
        }
        case RULE_NODE:
        case HBOX_NODE:
        case VBOX_NODE:
        case CHAR_NODE:
        case LIG_NODE:
        case KERN_NODE: {
          active_width[0] += p->width(state);
          break;
        }
        default:
          throw new GenericDiag("Unknown node type encountered in line break"
                                "ing algorithm.", DIAG_RENDER_ERR, BLAME_HERE);
      }
      prev_p = p;
      p = prev_p->link;
    }
    render.push();
    render.set_mode(HMODE);
    render.set_head(head);
    render.set_tail(tail);
    simple_line_break(state);
  }
public:
  LineBreaker(UniquePtr<State> &state) {
    active_width = cur_active_width = background = break_width
      = delta(scaled(0));

    sp left_skip = state->eqtb()[LEFT_SKIP_CODE].scaled;
    sp right_skip = state->eqtb()[RIGHT_SKIP_CODE].scaled;
    //sp hsize = state->eqtb()[HSIZE_CODE].scaled;
    background[DELTA_WIDTH] = left_skip + right_skip;

    minimum_demerits = PENALTY_AWFUL;
    for (unsigned i = 0; i < MAX_FITNESS; i++) {
      minimal_demerits[i] = PENALTY_AWFUL;
    }
  }
  
};

}

using namespace tex;

void tex::line_break(UniquePtr<State> &state) {
  LineBreaker breaker(state);
  breaker.break_line(state);
}