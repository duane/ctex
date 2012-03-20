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

#include <cstdlib>

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
  passive_node *parent, *link;
};

struct active_node {
  active_type type;
  active_node *link;
  union {
    struct {
      passive_node *passive;
      fitness_type fitness;
      int32_t demerits;
      int32_t line_number;
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
  RenderNode *head, *tail, *cur_p;

  active_node *active, *active_tail;
  passive_node *passive;

  int32_t threshold;
  int32_t old_line, easy_line;
  int32_t minimum_demerits;
  int32_t minimal_demerits[MAX_FITNESS];
  passive_node *best_place[MAX_FITNESS];
  int32_t best_place_line[MAX_FITNESS];

  bool final_pass;

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

    if (tail->type == GLUE_NODE) {
      tail->type = PENALTY_NODE;
      tail->penalty = PENALTY_INF;
    } else {
      tail->link = RenderNode::new_penalty(PENALTY_INF);
      tail = tail->link;
      tail->link = NULL;
    }

    // insert an active node at the beginning of the paragraph.
    act_head = new active_node;
    act_head->type = ACTIVE;
    act_head->link = NULL;
    act_head->passive = NULL;
    act_head->fitness = DECENT_FIT;
    act_head->demerits = 0;
    active_width = background;


    RenderNode *cur_p = head;
    RenderNode *prev_p = NULL;
    while (cur_p) {
      switch (cur_p->type) {
        case GLUE_NODE: {
          if (cur_p->glue.shrink_order != GLUE_NORMAL)
            throw new GenericDiag("Encountered infinite shrinkage in HMODE.",
                                  DIAG_RENDER_ERR, BLAME_HERE);
          if (prev_p && prev_p->type != GLUE_NODE) {
            RenderNode *break_rule
              = RenderNode::new_rule(scaled(1<<16), scaled(10<<16));
            break_rule->link = cur_p;
            prev_p->link = break_rule;
          }
          active_width[0] += cur_p->glue.width;
          active_width[1+cur_p->glue.stretch_order] += cur_p->glue.stretch;
          active_width[5] += cur_p->glue.shrink;
          break;
        }
        case RULE_NODE:
        case HBOX_NODE:
        case VBOX_NODE:
        case CHAR_NODE:
        case LIG_NODE:
        case KERN_NODE: {
          active_width[0] += cur_p->width(state);
          break;
        }
        default:
          throw new GenericDiag("Unknown node type encountered in line break"
                                "ing algorithm.", DIAG_RENDER_ERR, BLAME_HERE);
      }
      prev_p = cur_p;
      cur_p = prev_p->link;
    }
  }

  void try_break(UniquePtr<State> &state, int32_t penalty) {
    bool no_break_yet = true;
    if (penalty >= PENALTY_INF)
      return;
    if (penalty < PENALTY_BREAK)
      penalty = PENALTY_BREAK;

    cur_active_width = active_width;
    active_node *prev_prev_r, *prev_r, *r;
    prev_prev_r = prev_r = r = NULL;

    int32_t old_line = 0;

    sp line_width = scaled(0);

    while (true) {
      if (!prev_r)
        r = active;
      else
        r = prev_r->link;

      // TeX@832
      if (r && r->type == DELTA) {
        for (unsigned i = 0; i < 6; i++) {
          cur_active_width[i] += delta[i];
          prev_prev_r = prev_r;
          prev_r = r;
          continue;
        }
      }
      // End TeX@832

      // TeX@835
      int32_t line = r->line_number;
      if (line > old_line) {
        if (minimum_demerits < PENALTY_AWFUL
           && !(old_line == easy_line && r)) {
          // TeX@836
          if (no_break_yet) {
            // TeX@837
            no_break_yet = false;
            break_width = background;
            RenderNode *s = cur_p;
            while (s && s->type == GLUE_NODE) {
              break_width[0] -= s->glue.width;
              break_width[s->stretch_order + 1] -= s->glue.stretch;
              break_width[5] -= s->glue.shrink;
              s = s->link;
            }
            // end TeX@837
          }
          // TeX@843
          if (prev_r && prev_r->type == DELTA) {
            for (unsigned i = 0; i < 6; i++) {
              prev_r->delta[i] -= (cur_active_width[i] + break_width[i]);
            }
          } else if (prev_r == active) {
            active_width = break_width;
          } else {
            active_node *delta = new active_node;
            for (unsigned i = 0; i < 6; i++) {
              delta->delta[i] = (break_width[i] - cur_active_width[i]);
            }
            delta->type = DELTA;
            delta->link = r;
            if (prev_r)
              prev_r->link = delta;
            else
              active = delta;
            prev_prev_r = prev_r;
            prev_r = delta;
          }
          // end TeX@843;

          int32_t abs_adj_demerits = abs(state->eqtb()[ADJ_DEMERITS_CODE].i64);
          if (abs_adj_demerits >= (PENALTY_AWFUL - minimum_demerits))
            minimum_demerits = PENALTY_AWFUL - 1;
          else
            minimum_demerits += abs_adj_demerits;
          for (unsigned fit = 0; fit < MAX_FITNESS; fit++) {
            if (minimal_demerits[fit] < minimum_demerits) {
              // TeX@845
              passive_node *new_passive = new passive_node;
              new_passive->break_point = cur_p;
              new_passive->parent = best_place[fit];
              new_passive->link = passive;
              passive = new_passive;

              active_node *new_active = new active_node;
              new_active->type = active;
              new_active->passive = new_passive;
              new_active->line_number = best_place_line[fit] + 1;
              new_active->fitness = (fitness_type)fit;
              new_active->demerits = minimal_demerits[fit];
              if (prev_r)
                prev_r->link = new_active;
              else
                active = new_active;
              prev_r = new_active;
              new_active->link = r;
              // end TeX@845
              minimal_demerits[fit] = PENALTY_AWFUL;
            }
          }
          minimum_demerits = PENALTY_AWFUL;
          // end TeX@836;
        }
      }
      if (r->link == NULL)
        return;
      // TeX@850
      // Parshape/hanging indentation ignored for now.
      line_width = state->eqtb()[HSIZE_CODE].scaled;
      // End TeX@850
      // End TeX@835

      // TeX@851

      bool artificial_demerits = false;
      sp shortfall = line_width - cur_active_width[0];
      int32_t bad = 0;
      if (shortfall > 0) {
        // TeX@852
        // if we have infinite glue
        if (cur_active_width[DELTA_FIL]
            || cur_active_width[DELTA_FILL]
            || cur_active_width[DELTA_FILLL]) {
          badn = 0;
          fit_class = DECENT_FIT;
        } else if (shortfall > 7230584) {
          if (cur_active_width[DELTA_NORMAL] < 1663497) {
            bad = PENALTY_INF;
            fit_class = VERY_LOOSE_FIT;
          } else {
            bad = badness(shortfall, cur_active_width[DELTA_NORMAL]);
            if (bad > 12) {
              if (bad > 99)
                fit_class = VERY_LOOSE_FIT;
              else
                fit_class = LOOSE_FIT;
            } else
            fit_class = DECENT_FIT;
          }
        }
        // end TeX@852
      } else {
        // TeX@853
        // need to shrink.
        if (-shortfall > cur_active_width[DELTA_SHRINK])
          bad = PENALTY_INF;
        else
          bad = badness(-shortfall, cur_active_width[DELTA_SHRINK]);
        if (bad > 12)
          fit_class = TIGHT_FIT;
        else
          fit_class = DECENT_FIT;
        // end TeX@853
      }
      if (badness > PENALTY_INF || penalty == PENALTY_BREAK) {
        // TeX@854
        if (final_pass && minimum_demerits == PENALTY_AWFUL && !r->link)e
          artificial_demerits = true;
        else
          deactivate = true;
        // end TeX@854
      } else {
        prev_r = r;
        if (badness > threshold)
          continue;
        node_r_stays_active = true;
      }
      // TeX@855
      if (artificial_demerits)
        demerits = 0;
      else {
        // TeX@859
        demerits = line_penalty + bad;
        if (abs(demerits) > PENALTY_INF)
          demerits = PENALTY_AWFUL;
        else
          demerits *= demerits;
        if (penalty) {
          if (penalty > 0)
            demerits += penalty * penalty;
          else if (penalty > PENALTY_BREAK)
            demerits -= penalty * penalty;
        }
        if (abs(fit_class - r->fitness) > 1)
          d += state->eqtb()[ADJ_DEMERITS_CODE].i64;
        // End TeX@859
        demerits += r->demerits;
        if (demerits <= minimal_demerits[fit_class]) {
          minimal_demerits[fit_class] = demerits;
          best_place[fit_class] = r->active;
          best_place_line[fit_class] = line;
          if (demerits < minimum_demerits)
            minimum_demerits = demerits;
        }
      }
      // end TeX@855
      if (deactivate) {
        // TeX@860
        if (prev_r == NULL) {
          // TeX@861
          active = r->link;
          delete r;
          r = active;
          if (r->type == DELTA) {
            for (unsigned i = 0; i < 6; i++) {
              cur_active_width = r->delta[i];
            }
            active_width += cur_active_width;
            active = r->link;
            delete r;
            r = active;
          }
          // End TeX@861
        } else {
          prev_r->link = r->link;
          delete r;
          r = prev_r->link;
          if (prev_r->type == DELTA) {
            if (r == NULL) {
              for (unsigned i = 0; i < 6; i++) {
                cur_active_width[i] -= prev_r->delta[i];
              }
              if (prev_prev_r)
                prev_prev_r->link = prev_r->link;
              delete prev_r;
              prev_r = prev_prev_r;
            } else if (r->type == DELTA) {
              for (unsigned i = 0; i < 6; i++) {
                cur_active_width[i] += r->delta[i];
                prev_r->delta[i] += r->delta[i];
              }
              prev_r->link = r->link;
              delete r;
            }
          }
        }
        // end TeX@860
      }

      // EnD TeX@851
    }
  }

public:
  LineBreaker(UniquePtr<State> &state) {
    active_width = cur_active_width = background = break_width
      = delta(scaled(0));

    sp left_skip = state->eqtb()[LEFT_SKIP_CODE].scaled;
    sp right_skip = state->eqtb()[RIGHT_SKIP_CODE].scaled;
    //sp hsize = state->eqtb()[HSIZE_CODE].scaled;
    background[DELTA_WIDTH] = left_skip + right_skip;

    // TeX@834
    minimum_demerits = PENALTY_AWFUL;
    for (unsigned i = 0; i < MAX_FITNESS; i++) {
      minimal_demerits[i] = PENALTY_AWFUL;
    }
    // end TeX@834
  }
};

}

using namespace tex;

void tex::line_break(UniquePtr<State> &state) {
  LineBreaker breaker(state);
  breaker.break_line(state);
}